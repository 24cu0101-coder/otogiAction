// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectActionService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USelectActionService::USelectActionService()
{
	NodeName = TEXT("Update Enemy State");

	// サービスの実行間隔（秒）
	Interval = 0.2f;
	RandomDeviation = 0.05f;

	// キーのフィルタリング設定
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(USelectActionService, TargetActorKey), AActor::StaticClass());
	AIStateKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(USelectActionService, AIStateKey), StaticEnum<EEnemyState>());
}

// サービスの更新間隔ごとに呼ばれる処理
void USelectActionService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp) return;

	APawn* ControlledPawn = AIController->GetPawn();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));

	// 状況に応じてStateを選定
	EEnemyState NewState = EEnemyState::Idle;

	// ランダム抽選の候補となるStateをリストアップ（現在タスクが存在するもの）
	TArray<EEnemyState> CandidateStates = {
		EEnemyState::Chase,
		EEnemyState::Strafe,
	};

	// 直前と同じ行動を避ける設定が有効な場合、現在のStateを候補から除外
	if (bAvoidSameState && CandidateStates.Num() > 1)
	{
		const EEnemyState CurrentState = static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(AIStateKey.SelectedKeyName));
		CandidateStates.Remove(CurrentState);
	}

	// 候補の中からランダムに1つ選出
	const int32 RandomIndex = FMath::RandRange(0, CandidateStates.Num() - 1);
	NewState = CandidateStates[RandomIndex];

	////検証のためコメントアウト
	//{

		//if (ControlledPawn && TargetActor)
		//{
		//	const float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
		//	if (Distance <= AttackRange)
		//	{
		//		//4パターンほどに増やして、ランダムでパターンを選出するようにする
		//		NewState = EEnemyState::Attack;
		//	}
		//	else if(Distance <= StrafeRange)
		//	{
		//		NewState = EEnemyState::Strafe;
		//	}
		//	else
		//	{
		//		NewState = EEnemyState::Chase;
		//	}
		//}
	//}

	// Blackboardに選定結果（Enum）を書き込む
	BlackboardComp->SetValueAsEnum(AIStateKey.SelectedKeyName, static_cast<uint8>(NewState));
}

// サービスの更新間隔ごとに呼ばれる処理
void USelectActionService::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetActorKey.ResolveSelectedKey(*BBAsset);
		AIStateKey.ResolveSelectedKey(*BBAsset);
	}
}
