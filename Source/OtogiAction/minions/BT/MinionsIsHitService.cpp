#include "MinionsIsHitService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "OtogiAction/minions/MinionsCharacter.h"

UMinionsIsHitService::UMinionsIsHitService()
{
	NodeName = TEXT("Check Minions Hit");

	Interval = 0.2f;
	RandomDeviation = 0.0f;
}

void UMinionsIsHitService::TickNode(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory,float DeltaSeconds)
{

	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	UBlackboardComponent* BBComp =OwnerComp.GetBlackboardComponent();

	if (!BBComp)
	{
		return;
	}

	AAIController* AIController =OwnerComp.GetAIOwner();

	if (!AIController)
	{
		return;
	}

	AMinionsCharacter* Minions =Cast<AMinionsCharacter>(AIController->GetPawn());

	if (!Minions)
	{
		UE_LOG(LogTemp, Warning, TEXT("Minions == nullptr"));
		return;
	}


	BBComp->SetValueAsBool(IsHitKey.SelectedKeyName,Minions->GetIsHitFlg());
}