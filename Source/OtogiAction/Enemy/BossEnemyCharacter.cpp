// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Enemy/Component/BossEnemyHitReactionComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ABossEnemyCharacter::ABossEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;				//移動方向に向く
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);	//旋回速度（度/秒）
	}

	//敵キャラクターのBPにコンポーネントを追加
	AttackBuildComp = CreateDefaultSubobject<UAttackBuildComponent>(TEXT("AttackBuildComp"));
	AttackPressComp = CreateDefaultSubobject<UAttackPressComponent>(TEXT("AttackPressComp"));
	MoveBuildComp = CreateDefaultSubobject<UMoveBuildComponent>(TEXT("MoveBuildComp"));
	MovePressComp = CreateDefaultSubobject<UMovePressComponent>(TEXT("MovePressComp"));
	UtilityAIComp = CreateDefaultSubobject<UUtilityAIComponent>(TEXT("UtilityAIComp"));
	HitReactionComp = CreateDefaultSubobject<UBossEnemyHitReactionComponent>(TEXT("HitReactionComp"));

}

// Called when the game starts or when spawned
void ABossEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

}

//パンチアタックMontageの再生時間を返す
float ABossEnemyCharacter::GetPlayPunchAttackMontageTime()
{
	if (PunchAttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		//Montageの再生時間を秒数で返す
		return PlayAnimMontage(PunchAttackMontage);
	}
	return 0.f;
}

//ジャンプアタックMontageの再生時間を返す
float ABossEnemyCharacter::GetPlayJumpAttackMontageTime()
{
	if (JumpAttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		//Montageの再生時間を秒数で返す
		return PlayAnimMontage(JumpAttackMontage);
	}
	return 0.0f;
}

//敵のスピードをセットする
void ABossEnemyCharacter::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		//MaxWalkSpeedに新しい値を代入
		MoveComp->MaxWalkSpeed = NewSpeed;
	}
}

//敵のスピードをゲットする
float ABossEnemyCharacter::GetMovementSpeed()const
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		//設定されているMaxWalkSpeedを返す
		return MoveComp->MaxWalkSpeed;
	}
	return 0.0f;
}

//被弾しているかどうかを返す関数
bool ABossEnemyCharacter::GetIsHitFlg()const
{
	return IsHit;
}

//被弾のフラグをセットする関数
void ABossEnemyCharacter::SetIsHitFlg(bool NewFlg)
{
	IsHit = NewFlg;
}

//回転する関数
void ABossEnemyCharacter::RotateTowardsPlayer(float DeltaTime)
{
	//プレイヤー（Target）の取得
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	//自分とプレイヤーの位置を取得
	FVector MyLocation = GetActorLocation();
	FVector TargetLocation = PlayerPawn->GetActorLocation();

	//プレイヤーへの方向を示す回転（Rotator）を計算
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);

	//現在の回転を取得
	FRotator CurrentRotation = GetActorRotation();

	//指定した速度で補間
	const float InterpSpeed = GetCharacterMovement()->RotationRate.Yaw;
	FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

	//体が上下に傾かないよう Yaw（水平回転）のみを適用
	SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
}

//JumpAttackNotifyから呼び出す関数
void ABossEnemyCharacter::TriggerJumpAttack()
{
	//Notifyが作動したらdelegateを発火する
	OnJumpAttackNotify.Broadcast();
}

//第二形態かどうかを確認
bool ABossEnemyCharacter::GetIsPhaseTwo()
{
	return bIsPhaseTwo;
}

void ABossEnemyCharacter::StartPhaseTwo()
{
	bIsPhaseTwo = true;

	//AIコントローラー経由でBlackboardに第二形態フラグを設定
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
		{
			BBComp->SetValueAsBool(TEXT("IsPhaseTwo"), true);
		}
	}

	//Delegateを発火
	OnPhaseTwoStartedNotify.Broadcast();

	//BP側のイベントを呼び出す
	K2_OnPhaseTwoStarted();

	//第二形態移行時に呼ばれる関数
	ApplyPhaseTwoState();
}

void ABossEnemyCharacter::ApplyPhaseTwoState()
{
	//移動速度を変更
	SetMovementSpeed(PhaseTwoMaxWalkSpeed);

	//Niagaraエフェクトをメッシュのソケットに追従アタッチ
	if (PhaseTwoAuraEffect && GetMesh())
	{
		SpawnedAuraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			PhaseTwoAuraEffect,                 // スポーンするNiagara System
			GetMesh(),                          // アタッチ対象のコンポーネント（Skeletal Mesh）
			AuraSocketName,                     // アタッチ先のソケット/ボーン名
			FVector::ZeroVector,                // 相対位置オフセット
			FRotator::ZeroRotator,              // 相対回転オフセット
			EAttachLocation::KeepRelativeOffset,      // 位置・回転をソケットに完全に同期
			true,                               // AutoDestroy (エフェクト終了時に自動削除)
			true,                               // AutoActivate
			ENCPoolMethod::None,
			true
		);
	}
}

// Called every frame
void ABossEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABossEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.f)
	{
		return 0.f;
	}

	//BehaviorTreeと同期させるため被弾フラグを立てる
	IsHit = true;

	//ダメージをHPから差し引く
	CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.0f, MaxHP);

	UE_LOG(LogTemp, Warning, TEXT("CurrentHP: %f"), CurrentHP);

	//HPが0になったら死亡処理などを呼ぶ
	if (CurrentHP <= 0.0f)
	{
		IsHit = false;
		//死亡処理（Ragdoll化やデストロイなど）
		Destroy();
	}

	//第二形態への移行判定
	if (!bIsPhaseTwo && MaxHP > 0.0f && (CurrentHP / MaxHP) <= PhaseTwoHPThresholdRatio)
	{
		//第二形態ログ
		UE_LOG(LogTemp, Warning, TEXT("Phase 2"));

		StartPhaseTwo();

	}

	return ActualDamage;
}

