// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStrafeComponent.h"
#include "BossAIController.h"
#include "../PlayerCharacter/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UBossStrafeComponent::UBossStrafeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBossStrafeComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedOwnerPawn = Cast<APawn>(GetOwner());
	if (CachedOwnerPawn)
	{
		CachedAIController = Cast<ABossAIController>(CachedOwnerPawn->GetController());
	}

	SetTargetPlayer();
	
}

// Called every frame
void UBossStrafeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 無効化されている間は何もしない
	if (!bIsStrafeEnabled)
	{
		return;
	}

	// 未取得の参照があれば再取得
	if (!CachedAIController && CachedOwnerPawn)
	{
		CachedAIController = Cast<ABossAIController>(CachedOwnerPawn->GetController());
	}
	if (!TargetActor)
	{
		SetTargetPlayer();
	}

	if (!CachedOwnerPawn || !TargetActor)
	{
		return;
	}

	// 一定時間ごとに左右の移動方向を再抽選する
	if (ChangeDirectionInterval > 0.0f)
	{
		DirectionTimer += DeltaTime;
		if (DirectionTimer >= ChangeDirectionInterval)
		{
			DirectionTimer = 0.0f;
			PickRandomDirection();
		}
	}

	// プレイヤーを正面に捉えたまま、右方向ベクトルへ移動入力を送る（＝円周上の横移動）
	const FVector RightVector = CachedOwnerPawn->GetActorRightVector();
	CachedOwnerPawn->AddMovementInput(RightVector * StrafeDirection, StrafeSpeedScale);
}

//見合う行動を取るかどうか
void UBossStrafeComponent::SetStrafeEnabled(bool bEnable)
{
	bIsStrafeEnabled = bEnable;

	if (!CachedAIController && CachedOwnerPawn)
	{
		CachedAIController = Cast<ABossAIController>(CachedOwnerPawn->GetController());
	}

	if (bIsStrafeEnabled)
	{
		// 開始時に左右どちらへ動くか決定
		PickRandomDirection();
		DirectionTimer = 0.0f;

		// プレイヤーの方を常に正面に捉えるようフォーカスを設定
		if (CachedAIController && TargetActor)
		{
			CachedAIController->SetFocus(TargetActor);
		}

		// 横歩き時に進行方向へ体が回転しないよう制御モードを切り替え
		if (ACharacter* OwnerChar = Cast<ACharacter>(CachedOwnerPawn))
		{
			if (UCharacterMovementComponent* MoveComp = OwnerChar->GetCharacterMovement())
			{
				MoveComp->bOrientRotationToMovement = false;
				MoveComp->bUseControllerDesiredRotation = true;
			}
		}
	}
	else
	{
		// 無効化時はフォーカスを解除し、通常の移動回転設定に戻す
		if (CachedAIController)
		{
			CachedAIController->ClearFocus(EAIFocusPriority::Gameplay);
		}

		if (ACharacter* OwnerChar = Cast<ACharacter>(CachedOwnerPawn))
		{
			if (UCharacterMovementComponent* MoveComp = OwnerChar->GetCharacterMovement())
			{
				MoveComp->bOrientRotationToMovement = true;
				MoveComp->bUseControllerDesiredRotation = false;
			}
		}
	}
}

//プレイヤー情報を取得
void UBossStrafeComponent::SetTargetPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		TargetActor = Cast<APlayerCharacter>(PlayerPawn);
	}
}

//進行方向をランダムで取得する
void UBossStrafeComponent::PickRandomDirection()
{
	StrafeDirection = FMath::RandBool() ? 1.0f : -1.0f;
}

