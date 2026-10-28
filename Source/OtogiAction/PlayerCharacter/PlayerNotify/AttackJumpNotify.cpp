// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackJumpNotify.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/NormalAttack/NormalComboAttackComponent.h"

void UAttackJumpNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	AActor* OwnerAtcotr = MeshComp->GetOwner();

	if(OwnerAtcotr)
	{
		UNormalComboAttackComponent* AttackComp = OwnerAtcotr->FindComponentByClass<UNormalComboAttackComponent>();
		if (AttackComp)
		{
			//AttackComp->AttackJump(UpPower);

			AttackComp->AttackMove(forwardPower,UpPower);
		}


	}
}
