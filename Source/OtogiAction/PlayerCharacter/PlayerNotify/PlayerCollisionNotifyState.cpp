// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCollisionNotifyState.h"
#include "GameFramework/Character.h"
#include "../../Component/Collision/SphereCollisionComponent.h"

void UPlayerCollisionNotifyState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (USphereCollisionComponent* CollComp = Cast<USphereCollisionComponent>(MeshComp->GetOwner()))
	{
		CollComp->ExcuteAreaAttack(Radius, TargetTag, Damage);
	}
}
