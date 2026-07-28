// Fill out your copyright notice in the Description page of Project Settings.


#include "TypeAheadNotifyState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/NormalAttack/NormalComboAttackComponent.h"

void UTypeAheadNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UNormalComboAttackComponent* NAttackComp = OwnerActor->FindComponentByClass<UNormalComboAttackComponent>())
		{
			if (UAbilitySystemComponent* ASComponent = NAttackComp->GetNAttackACS())
			{
				ASComponent->AddLooseGameplayTag(TypeAheadTag);
			}
		}
	}
}

void UTypeAheadNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UNormalComboAttackComponent* NAttackComp = Owner->FindComponentByClass<UNormalComboAttackComponent>())
		{
			if (UAbilitySystemComponent* ASC = NAttackComp->GetNAttackACS())
			{
				ASC->RemoveLooseGameplayTag(TypeAheadTag);
			}
		}
	}
}
