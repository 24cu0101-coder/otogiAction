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

	CachedOwnerComp = &OwnerComp;

	//コンポーネントを動的に生成して、Pawnにアタッチする
	NewAttack = NewObject<UEnemyAttackBaseComponent>(EnemyPawn, AttackClass);
	if (NewAttack)
	{
		NewAttack->RegisterComponent();

		//プレイヤーを注視
		AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(PlayerActorKey.SelectedKeyName));
		if (TargetActor)
		{
			EnemyController->SetFocus(TargetActor);
		}

		//終了イベントをバインド
		NewAttack->OnAttackFinished.AddDynamic(this, &UAttackTask::OnAttackCompleted);

		BBComp->SetValueAsBool(CanPunchAttackKey.SelectedKeyName, true);

		//攻撃を実行
		NewAttack->ExecuteAttack();

		//攻撃が終わるまでここで待機させる
		return EBTNodeResult::InProgress;
	}


	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UAttackTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		//注視を解除
		AIController->ClearFocus(EAIFocusPriority::Gameplay);

		ACharacter* EnemyCharacter = Cast<ACharacter>(AIController->GetPawn());
		if (EnemyCharacter && EnemyCharacter->GetMesh())
		{
			if (UAnimInstance* AnimInstance = EnemyCharacter->GetMesh()->GetAnimInstance())
			{
				// 割込みが入ったら現在再生中の攻撃モンタージュを即座に停止する
				AnimInstance->Montage_Stop(0.1f, nullptr); // 0.1秒でブレンドアウト
			}
		}
	}

	if (NewAttack)
	{
		NewAttack->OnAttackFinished.RemoveAll(this);
		NewAttack->DestroyComponent();
		NewAttack = nullptr;
	}

	//Blackboardの攻撃可能フラグを下げておく
	if (BBComp)
	{
		BBComp->SetValueAsBool(CanPunchAttackKey.SelectedKeyName, false);
	}

	//明示的にBTへ「中断完了」を伝える
	FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);

	return EBTNodeResult::Aborted;
}

//攻撃終了時
void UAttackTask::OnAttackCompleted(bool bSuccess)
{
	//BTにタスクが完了したことを通知
	EBTNodeResult::Type Result = bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	FinishLatentTask(*CachedOwnerComp, Result);

	AAIController* AIController = CachedOwnerComp->GetAIOwner();
	if (AIController)
	{
		//注視を解除
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	if (BBComp)
	{
		//BTで指定したCanPunchAttackKeyをfalseに
		BBComp->SetValueAsBool(CanPunchAttackKey.SelectedKeyName, false);
	}
}
