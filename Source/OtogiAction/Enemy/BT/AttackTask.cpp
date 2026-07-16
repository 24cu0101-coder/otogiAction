// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackTask.h"
#include "AIController.h"
#include "../EnemyAttackBaseComponent.h"
#include "../BossEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UAttackTask::UAttackTask()
{
	NodeName = TEXT("Punch Attack Task");

	//時間を要するタスクであることを示すフラグを立てる
	bNotifyTick = false;
}

//タスク開始
EBTNodeResult::Type UAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!AttackClass) return EBTNodeResult::Failed;

	AAIController* EnemyController = OwnerComp.GetAIOwner();
	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;
	if (!EnemyPawn) return EBTNodeResult::Failed;

	//AIControllerからBlackboardコンポーネントを取得
	BBComp = EnemyController->GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;

	//PunchAttackKeyがfalseだったら
	if (!BBComp->GetValueAsBool(CanPunchAttackKey.SelectedKeyName)) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;

	//コンポーネントを動的に生成して、Pawnにアタッチする
	UEnemyAttackBaseComponent* NewAttack = NewObject<UEnemyAttackBaseComponent>(EnemyPawn, AttackClass);
	if (NewAttack)
	{
		NewAttack->RegisterComponent();

		//終了イベントをバインド
		NewAttack->OnAttackFinished.AddDynamic(this, &UAttackTask::OnAttackCompleted);

		BBComp->SetValueAsBool(TEXT("CanAttack"), true);

		//攻撃を実行
		NewAttack->ExecuteAttack();

		//攻撃が終わるまでここで待機させる
		return EBTNodeResult::InProgress;
	}


    return EBTNodeResult::Failed;
}

//攻撃終了時
void UAttackTask::OnAttackCompleted(bool bSuccess)
{
	//BTにタスクが完了したことを通知
	EBTNodeResult::Type Result = bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	FinishLatentTask(*CachedOwnerComp, Result);

	//BTで指定したCanPunchAttackKeyをfalseに
	BBComp->SetValueAsBool(CanPunchAttackKey.SelectedKeyName, false);

}
