// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBuildComponent.h"
#include "../EnemyStateSubsystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

UMoveBuildComponent::UMoveBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 通常追跡の標準レンジ設定（例: 中〜近距離で評価が高くなる）
	MinRange = 200.f;
	MaxRange = 600.f;
	FadeOutRange = 1000.f;
	BasePriority = 0.5f; // ダッシュより優先度は低めに設定
}

void UMoveBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		CachedAIController = Cast<AAIController>(OwnerPawn->GetController());

		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn))
		{
			CachedMovementComp = OwnerCharacter->GetCharacterMovement();
		}
	}
}

void UMoveBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

void UMoveBuildComponent::SetEnemyState()
{
	if (UWorld* World = GetWorld())
	{
		// 2. GameInstance を取得して Subsystem を呼び出す
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UEnemyStateSubsystem* StateSubsystem = GI->GetSubsystem<UEnemyStateSubsystem>())
			{
				// 状態の変更
				StateSubsystem->CurrentState = EEnemyState::MoveBuild;
			}
		}
	}

}

bool UMoveBuildComponent::StartTracking()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return false;

	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!AIController || !TargetPawn) return false;

	// 移動速度の設定
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerPawn))
	{
		if (UCharacterMovementComponent* MoveComp = OwnerCharacter->GetCharacterMovement())
		{
			MoveComp->MaxWalkSpeed = NormalWalkSpeed;
		}
	}

	// 移動開始
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(TargetPawn, MinRange);
	bIsTracking = (MoveResult != EPathFollowingRequestResult::Type::Failed);

	// 失敗じゃなければ追跡中フラグを立てる
	bIsTracking = (MoveResult != EPathFollowingRequestResult::Failed);

	// すでに目的地にいる場合は追跡不要として false にする
	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		bIsTracking = false;
	}

	return bIsTracking;
}

void UMoveBuildComponent::UpdateTrackingStatus()
{
	if (!bIsTracking || !CachedAIController) return;

	//AIControllerのPathFollowingComponentから現在の移動状態を取得
	UPathFollowingComponent* PathComp = CachedAIController->GetPathFollowingComponent();
	if (PathComp)
	{
		//移動が完了している場合
		if (PathComp->GetStatus() == EPathFollowingStatus::Idle)
		{
			bIsTracking = false;
		}
	}
}

void UMoveBuildComponent::FinishTracking()
{
	if (CachedAIController)
	{
		CachedAIController->StopMovement();
	}
	bIsTracking = false;
}
