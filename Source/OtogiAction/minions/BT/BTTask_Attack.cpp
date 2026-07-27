#include "BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "../MinionsCharacter.h"
#include "../minionsAttackComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("===== BT HIT REACTION TASK ====="));

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AMinionsCharacter* Minions =
		Cast<AMinionsCharacter>(AIController->GetPawn());

	if (!Minions)
	{
		return EBTNodeResult::Failed;
	}

	if (!Minions->AttackComponent)
	{
		return EBTNodeResult::Failed;
	}

	Minions->AttackComponent->Attack();

	return EBTNodeResult::Succeeded;
}