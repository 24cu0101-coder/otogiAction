// Fill out your copyright notice in the Description page of Project Settings.


#include "BossNomalAttackTask.h"
#include "../../Main/BossAIController.h"
#include "../../Component/BossBaseAttackComponent.h"
#include "../../Main/BossCharacter.h"

UBossNomalAttackTask::UBossNomalAttackTask()
{
	NodeName = TEXT("Execute Attack");

	// AIごとにインスタンスを独立させる（メンバ変数にキャッシュを持つため必須）
	bCreateNodeInstance = true;
}

//タスク実行時関数
EBTNodeResult::Type UBossNomalAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 親クラス型で検索することで、MeleeAttackComponent などどの派生クラスでも取得できる
	UBossBaseAttackComponent* AttackComp = ControlledPawn->FindComponentByClass<UBossBaseAttackComponent>();
	if (!AttackComp)
	{
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = &OwnerComp;
	CachedAttackComp = AttackComp;

	// 終了イベントを購読（二重登録防止のため一度解除してから登録）
	AttackComp->OnAttackEnded.RemoveDynamic(this, &UBossNomalAttackTask::HandleAttackEnded);
	AttackComp->OnAttackEnded.AddDynamic(this, &UBossNomalAttackTask::HandleAttackEnded);

	// 攻撃開始（ポリモーフィズムにより子クラスの StartAttack_Implementation が実行される）
	AttackComp->StartAttack();

	// 攻撃が開始されなかった（条件を満たさず即終了した）場合は Failed を返す
	if (!AttackComp->IsAttacking())
	{
		AttackComp->OnAttackEnded.RemoveDynamic(this, &UBossNomalAttackTask::HandleAttackEnded);
		return EBTNodeResult::Failed;
	}

	// アニメーション終了までタスクを「実行中」にして待機
	return EBTNodeResult::InProgress;
}

//タスク中断関数
EBTNodeResult::Type UBossNomalAttackTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ノックバックやターゲット喪失などでBTが中断された場合のクリーンアップ
	if (CachedAttackComp.IsValid())
	{
		CachedAttackComp->OnAttackEnded.RemoveDynamic(this, &UBossNomalAttackTask::HandleAttackEnded);
		CachedAttackComp->EndAttack();
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

//攻撃終了通知を受け取るコールバック
void UBossNomalAttackTask::HandleAttackEnded()
{
	if (CachedAttackComp.IsValid())
	{
		CachedAttackComp->OnAttackEnded.RemoveDynamic(this, &UBossNomalAttackTask::HandleAttackEnded);
	}

	if (CachedOwnerComp.IsValid())
	{
		// 待機していたBT Taskを「成功」として完了させ、次のノードへ進める
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}
