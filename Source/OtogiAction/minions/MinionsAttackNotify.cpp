#include "MinionsAttackNotify.h"

#include "MinionsCharacter.h"

#include "OtogiAction/Component/Collision/SphereCollisionComponent.h"


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


	USphereCollisionComponent* SphereCollision =
		Minions->FindComponentByClass<USphereCollisionComponent>();


	if (!SphereCollision)
	{
		UE_LOG(LogTemp, Error,
			TEXT("SphereCollisionComponent Not Found"));

		return;
	}


	SphereCollision->ExcuteAreaAttack(
		Radius,
		FName("Player"),
		Damage,
		StunPoint, 
		0.f,
		1.f,             
		ForwardOffset,
		SideOffset
	);


	UE_LOG(LogTemp, Warning,
		TEXT("Minions Attack Notify"));
}