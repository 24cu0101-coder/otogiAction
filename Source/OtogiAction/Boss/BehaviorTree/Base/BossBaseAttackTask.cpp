// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBaseAttackTask.h"
#include "../../Component/Base/BossBaseAttackComponent.h"
#include "../../Main/BossAIController.h"

UBossBaseAttackTask::UBossBaseAttackTask()
{
	// 状態（CachedOwnerCompなど）をメンバ変数に保持するため、ノードをインスタンス化する設定が安全です
	bCreateNodeInstance = true;
}

//タスク実行時関数
EBTNodeResult::Type UBossBaseAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AIコントローラーを経由してボス本体取得
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	//ボス本体にアタッチされているコンポーネント取得
	UBossBaseAttackComponent* AttackComp = ControlledPawn->FindComponentByClass<UBossBaseAttackComponent>();
	if (!AttackComp)
	{
		return EBTNodeResult::Failed;
	}

	// 参照をキャッシュ
	CachedOwnerComp = &OwnerComp;
	CachedAttackComp = AttackComp;

	// 終了通知デリゲートをバインド（※デリゲート名は実際のコンポーネントに合わせて変更してください）
	AttackComp->OnAttackEnded.AddDynamic(this, &UBossBaseAttackTask::HandleAttackEnded);

	// 子クラス固有の攻撃処理を実行
	if (!StartAttack(AttackComp))
	{
		// 攻撃の開始に失敗した場合はデリゲートを解除して即座に失敗を返す
		CleanupTask();
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

//タスク中断関数
EBTNodeResult::Type UBossBaseAttackTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedAttackComp.IsValid())
	{
		// 必要であればコンポーネント側の攻撃中断処理を呼ぶ
		// CachedAttackComp->CancelAttack();
	}

	CleanupTask();
	return EBTNodeResult::Aborted;
}

FString UBossBaseAttackTask::GetStaticDescription() const
{
	// BTエディタ上のノード下部に、どのコンポーネントが設定されているか表示する
	const FString CompName = TargetAttackComponentClass ? TargetAttackComponentClass->GetName() : TEXT("None");
	return FString::Printf(TEXT("Target Component: %s"), *CompName);
}

//攻撃終了通知を受け取るコールバック関数
void UBossBaseAttackTask::HandleAttackEnded()
{
	UBehaviorTreeComponent* OwnerComp = CachedOwnerComp.Get();

	// FinishLatentTask を呼ぶ前にクリーンアップを行う
	CleanupTask();

	if (OwnerComp)
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBossBaseAttackTask::CleanupTask()
{
	if (CachedAttackComp.IsValid())
	{
		// バインド解除（忘れやすいので一箇所にまとめる）
		CachedAttackComp->OnAttackEnded.RemoveDynamic(this, &UBossBaseAttackTask::HandleAttackEnded);
	}

	CachedOwnerComp.Reset();
	CachedAttackComp.Reset();
}
