#include "MinionsAttackNotify.h"

#include "MinionsCharacter.h"
#include "OtogiAction/Component/Collision/SphereCollisionComponent.h"


void UMinionsAttackNotify::Notify(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation)
{

	if (!MeshComp)
	{
		return;
	}


	AMinionsCharacter* Minion =Cast<AMinionsCharacter>(MeshComp->GetOwner());


	if (!Minion)
	{
		return;
	}



	// 攻撃中チェック


	if (!Minion->IsAttacking())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Attack Notify Cancelled"));

		return;
	}



	USphereCollisionComponent* SphereCollision =Minion->FindComponentByClass<USphereCollisionComponent>();


	if (!SphereCollision)
	{
		UE_LOG(LogTemp, Error,
			TEXT("SphereCollisionComponent Not Found"));

		return;
	}



	// 攻撃判定

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
		TEXT("Minions Attack Hit"));

}