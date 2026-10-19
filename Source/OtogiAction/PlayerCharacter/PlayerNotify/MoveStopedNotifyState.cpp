// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveStopedNotifyState.h"
#include "../PlayerComponent/Move/MoveComponent.h"
#include "../PlayerComponent/NormalAttack/NormalComboAttackComponent.h"


void UMoveStopedNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UMoveComponent* MoveComp = Owner->FindComponentByClass<UMoveComponent>())
		{
			MoveComp->SetMovementLock(true);
		}
	}
}

void UMoveStopedNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UMoveComponent* MoveComp = Owner->FindComponentByClass<UMoveComponent>())
		{
			MoveComp->SetMovementLock(false);
		}

		// 髙山記述(なるべく変更しないでください)
		if (UNormalComboAttackComponent* AttackComp = Owner->FindComponentByClass<UNormalComboAttackComponent>())
		{
			AttackComp->SetCancelAttackFlag();
		}

	}
}
