// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePressComponent.h"
#include "../EnemyStateSubsystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

UMovePressComponent::UMovePressComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ダッシュは遠距離（600〜1500）のときに評価が高くなるよう設定
	MinRange = 600.0f;
	MaxRange = 1500.0f;
	FadeOutRange = 2500.0f;
	BasePriority = 0.8f; // 通常移動より優先度を高く設定
}

void UMovePressComponent::SetEnemyState()
{
	if (UWorld* World = GetWorld())
	{
		// 2. GameInstance を取得して Subsystem を呼び出す
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UEnemyStateSubsystem* StateSubsystem = GI->GetSubsystem<UEnemyStateSubsystem>())
			{
				// 状態の変更
				StateSubsystem->CurrentState = EEnemyState::MovePress;
			}
		}
	}

}

bool UMovePressComponent::StartDashTracking()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return false;

	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!AIController || !TargetPawn) return false;

	// ダッシュ速度の設定
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn))
	{
		if (UCharacterMovementComponent* MoveComp = OwnerCharacter->GetCharacterMovement())
		{
			MoveComp->MaxWalkSpeed = DashSpeed;
		}
	}

	// 移動開始
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(TargetPawn, 200.0f);
	bIsDashTracking = (MoveResult != EPathFollowingRequestResult::Type::Failed);

	return bIsDashTracking;
}

void UMovePressComponent::FinishDashTracking()
{
	bIsDashTracking = false;
}