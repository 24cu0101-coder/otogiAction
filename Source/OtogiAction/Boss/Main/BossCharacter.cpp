// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "BossAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "BrainComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//使用する AIController のクラスを指定
	AIControllerClass = ABossAIController::StaticClass();

	//自動的にPossessされるタイミングを設定
	// EAutoPossessAI::PlacedInWorldOrSpawned : レベル配置時・Spawn時のどちらでも自動生成＆Possess
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//コンポーネントのアタッチ
	ChaseComponent = CreateDefaultSubobject<UBossChaseComponent>(TEXT("ChaseComponent"));
	StrafeComponent = CreateDefaultSubobject<UBossStrafeComponent>(TEXT("StrafeComponent"));

}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!BossTag.IsNone())
	{
		Tags.AddUnique(BossTag);
	}
}

//被弾時の処理
float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 親クラスの処理を実行して実際のダメージ量を取得
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)

		// HPを減算（Header側で CurrentHP や MaxHP が定義されている前提）
		CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.0f, MaxHP);

	if (GEngine)
	{
		//現在HPをデバッグログで表示
		FString DebugMsg = FString::Printf(TEXT("HP: %.1f"), CurrentHP);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, DebugMsg);
	}

	// HPが0になったら死亡処理
	if (CurrentHP <= 0.0f)
	{
		Die();
	}

	return ActualDamage;
}

//死亡時の処理
void ABossCharacter::Die()
{
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;

	//AIコントローラーの思考と移動を停止
	if (ABossAIController* BossAI = Cast<ABossAIController>(GetController()))
	{
		if (BossAI->GetBrainComponent())
		{
			BossAI->GetBrainComponent()->StopLogic(TEXT("Boss Died"));
		}
		BossAI->StopMovement();
	}

	//独自の移動コンポーネントとCharacterMovementを停止
	if (ChaseComponent)
	{
		ChaseComponent->SetComponentTickEnabled(false);
	}
	if (StrafeComponent)
	{
		StrafeComponent->SetComponentTickEnabled(false);
	}
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DisableMovement();
	}

	//カプセルの当たり判定を無効化（プレイヤーの移動や攻撃の邪魔にならないようにする）
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//死亡モンタージュの再生
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance && DeathMontage)
	{
		const float Duration = AnimInstance->Montage_Play(DeathMontage, 1.0f);
		if (Duration > 0.0f)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ABossCharacter::OnDeathMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathMontage);
			return;
		}
	}

	//モンタージュが未設定、または再生に失敗した場合は即座に消滅
	Destroy();
}

// モンタージュ再生終了時のコールバック
void ABossCharacter::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 倒れきった状態で3秒間残してからアクターを破棄する
	SetLifeSpan(3.0f);
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

