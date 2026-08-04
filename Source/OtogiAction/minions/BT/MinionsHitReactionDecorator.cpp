#include "MinionsHitReactionDecorator.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "OtogiAction/minions/MinionsCharacter.h"

UMinionsHitReactionDecorator::UMinionsHitReactionDecorator()
{
	NodeName = TEXT("Minions Hit Reaction");
}

bool UMinionsHitReactionDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	AMinionsCharacter* Minions =
		Cast<AMinionsCharacter>(AIController->GetPawn());

	if (!Minions)
	{
		return false;
	}

	const UBlackboardComponent* BBComp =OwnerComp.GetBlackboardComponent();

	if (!BBComp)
	{
		return false;
	}

	return BBComp->GetValueAsBool(IsHitKey.SelectedKeyName);
}