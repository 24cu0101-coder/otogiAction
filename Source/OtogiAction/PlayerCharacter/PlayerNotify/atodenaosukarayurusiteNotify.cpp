// Fill out your copyright notice in the Description page of Project Settings.


#include "atodenaosukarayurusiteNotify.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/StrongAttack/IaiAttackComponent.h"

void UatodenaosukarayurusiteNotify::Notify(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	
	AActor* OwnerAtcotr = MeshComp->GetOwner();
	
	if (OwnerAtcotr)
	{
		m_iaiAttackComp = OwnerAtcotr->FindComponentByClass<UIaiAttackComponent>();
	}

	if (m_iaiAttackComp)
	{
		m_iaiAttackComp->ChargeDamageReset();
	}


}