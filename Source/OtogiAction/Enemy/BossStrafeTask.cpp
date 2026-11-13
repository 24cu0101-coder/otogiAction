// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStrafeTask.h"
#include "AIController.h"
#include "BossCharacter.h"
#include "BossStrafeComponent.h"

UBossStrafeTask::UBossStrafeTask()
{
	NodeName = TEXT("Strafe Task");
}

EBTNodeResult::Type UBossStrafeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBossStrafeComponent* StrafeComp = GetStrafeComponent(OwnerComp);
	if (!StrafeComp)
	{
		return EBTNodeResult::Failed;
	}

	// コンポーネントの見合い処理を有効化
	StrafeComp->SetStrafeEnabled(true);

	// StateがStrafeである間は継続実行するため InProgress を返す
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBossStrafeTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Stateが切り替わって中断されたら、見合い処理を無効化してフォーカス解除
	if (UBossStrafeComponent* StrafeComp = GetStrafeComponent(OwnerComp))
	{
		StrafeComp->SetStrafeEnabled(false);
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

UBossStrafeComponent* UBossStrafeTask::GetStrafeComponent(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return nullptr;

	ABossCharacter* BossChar = Cast<ABossCharacter>(AIController->GetPawn());
	if (!BossChar) return nullptr;

	return BossChar->FindComponentByClass<UBossStrafeComponent>();
}
