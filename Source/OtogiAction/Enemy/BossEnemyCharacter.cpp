// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Component/UHitReactionBaseComponent.h"
#include "BossEnemyHitReactionComponent.h"

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

	//ヒットリアクションコンポーネント
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

	return ActualDamage;
}

