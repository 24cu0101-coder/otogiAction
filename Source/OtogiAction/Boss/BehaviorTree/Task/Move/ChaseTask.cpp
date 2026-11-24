// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseTask.h"
#include "AIController.h"
#include "../../../Main/BossCharacter.h"
#include "../../../Component/Move/BossChaseComponent.h"

UChaseTask::UChaseTask() 
{
    // Behavior Treeエディタ上に表示されるノード名
    NodeName = TEXT("Chase Task");

	bNotifyTick = true;
}

// タスク開始時に呼ばれる
EBTNodeResult::Type UChaseTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBossChaseComponent* ChaseComp = GetChaseComponent(OwnerComp);
	if (!ChaseComp)
	{
		return EBTNodeResult::Failed;
	}

	// すでに一定距離内にいる場合は即座にタスク完了とする
	if (ChaseComp->IsWithinAcceptanceRadius(CompleteDistance))
	{
		return EBTNodeResult::Succeeded;
	}

	// コンポーネント側の停止距離と同期させる
	ChaseComp->AcceptanceRadius = CompleteDistance;

	// 追跡フラグを有効化し、即座に1回目の追跡移動を開始
	ChaseComp->SetTrackingEnabled(true);
	ChaseComp->TrackingPlayer();

	// Stateが「Chase」である限りこのタスクを維持し続けるため InProgress を返す
	return EBTNodeResult::InProgress;
}

void UChaseTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBossChaseComponent* ChaseComp = GetChaseComponent(OwnerComp);
	if (!ChaseComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 一定距離以内に近づいたら追跡を止めてタスクを成功(完了)にする
	if (ChaseComp->IsWithinAcceptanceRadius(CompleteDistance))
	{
		ChaseComp->SetTrackingEnabled(false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

// Stateが変わってDecoratorに中断された時に呼ばれる
EBTNodeResult::Type UChaseTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// StateがAttackやIdleに変わってタスクが中断されたら、追跡をオフにして移動を止める
	if (UBossChaseComponent* ChaseComp = GetChaseComponent(OwnerComp))
	{
		ChaseComp->SetTrackingEnabled(false);
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

// ボスキャラからChaseComponentを取得するヘルパー関数
UBossChaseComponent* UChaseTask::GetChaseComponent(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return nullptr;

	// 制御しているPawnをABossCharacterにキャスト
	ABossCharacter* BossChar = Cast<ABossCharacter>(AIController->GetPawn());
	if (!BossChar) return nullptr;

	// コンポーネントを取得（FindComponentByClass または BossChar の Getter を使用）
	return BossChar->FindComponentByClass<UBossChaseComponent>();
}
