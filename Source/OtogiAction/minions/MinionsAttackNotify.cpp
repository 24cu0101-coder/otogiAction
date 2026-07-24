#include "MinionsAttackNotify.h"
#include "MinionsCharacter.h"
#include "minionsAttackComponent.h"

#include "GameFramework/Actor.h"


void UMinionsAttackNotify::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}


	AMinionsCharacter* Minions =
		Cast<AMinionsCharacter>(MeshComp->GetOwner());


	if (!Minions)
	{
		return;
	}


	UminionsAttackComponent* AttackComponent =
		Minions->FindComponentByClass<UminionsAttackComponent>();


	if (!AttackComponent)
	{
		UE_LOG(LogTemp, Error,
			TEXT("Minions AttackComponent Not Found"));

		return;
	}


	AttackComponent->ExecuteAttackHit();

	UE_LOG(LogTemp, Warning,
		TEXT("Minions Attack Notify"));
}