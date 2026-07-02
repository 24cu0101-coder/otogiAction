// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackTask.h"
#include "AIController.h"
#include "BossEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UAttackTask::UAttackTask()
{
	NodeName = TEXT("C++ Attack Task");

	//時間を要するタスクであることを示すフラグを立てる
	bNotifyTick = false;
}

EBTNodeResult::Type UAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ABossEnemyCharacter* EnemyChar = Cast<ABossEnemyCharacter>(AIController->GetPawn());
	if (!EnemyChar) return EBTNodeResult::Failed;

	//攻撃中は注視を解除
	AIController->ClearFocus(EAIFocusPriority::Gameplay);

	//タスクが実行中であることを返す
	return EBTNodeResult::InProgress;
}

//終了時に流れる関数
void UAttackTask::OnAttackAnimationFinished(TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompPtr)
{
	if (OwnerCompPtr.IsValid())
	{
		UBehaviorTreeComponent* BTComp = OwnerCompPtr.Get();
		AAIController* AIController = BTComp->GetAIOwner();

		if (AIController)
		{
			UBlackboardComponent* BBComp = BTComp->GetBlackboardComponent();
			if (BBComp)
			{
				AActor* PlayerActor = Cast<AActor>(BBComp->GetValueAsObject(FName("PlayerActor")));
				if (PlayerActor)
				{
					AIController->SetFocus(PlayerActor);
				}
			}
		}

		//Behavior Treeに終了したと通知する
		FinishLatentTask(*OwnerCompPtr.Get(), EBTNodeResult::Succeeded);
	}
}
