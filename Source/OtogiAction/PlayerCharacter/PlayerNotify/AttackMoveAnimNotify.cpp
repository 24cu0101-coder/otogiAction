// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMoveAnimNotify.h"

void UAttackMoveAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{

	}
}