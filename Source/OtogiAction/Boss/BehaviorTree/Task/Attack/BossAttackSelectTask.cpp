// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttackSelectTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBossAttackSelectTask::UBossAttackSelectTask()
{
	NodeName = TEXT("Select Next Action");

	// キーのフィルタリング設定
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBossAttackSelectTask, TargetActorKey), AActor::StaticClass());
	AIStateKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBossAttackSelectTask, AIStateKey), StaticEnum<EEnemyState>());

	// デフォルトの抽選候補
	CandidateStates = {
		EEnemyState::NormalAttack,
		EEnemyState::ComboAttack
	};
}

EBTNodeResult::Type UBossAttackSelectTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// エディタで設定された CandidateStates をローカル変数にコピー
	TArray<EEnemyState> AvailableStates = CandidateStates;
	if (AvailableStates.IsEmpty())
	{
		return EBTNodeResult::Failed;
	}

	// 直前と同じ行動を避ける設定が有効な場合、現在のStateを候補から除外
	if (bAvoidSameState && AvailableStates.Num() > 1)
	{
		const EEnemyState CurrentState = static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(AIStateKey.SelectedKeyName));
		AvailableStates.Remove(CurrentState);
	}

	// 候補の中からランダムに1つ選出
	const int32 RandomIndex = FMath::RandRange(0, AvailableStates.Num() - 1);
	const EEnemyState NewState = AvailableStates[RandomIndex];

	// Blackboardに選定結果（Enum）を書き込む
	BlackboardComp->SetValueAsEnum(AIStateKey.SelectedKeyName, static_cast<uint8>(NewState));

	return EBTNodeResult::Succeeded;
}

void UBossAttackSelectTask::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetActorKey.ResolveSelectedKey(*BBAsset);
		AIStateKey.ResolveSelectedKey(*BBAsset);
	}
}
