// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferNotifyState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/NormalAttack/NormalComboAttackComponent.h"

void UInputBufferNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor))
		{
			// 範囲に入ったらタグを付与
			ASC->AddLooseGameplayTag(TypeAheadTag);
		}
	}
}

void UInputBufferNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
		{
			// 範囲を出たらタグを削除
			ASC->RemoveLooseGameplayTag(TypeAheadTag);
		}
	}
}
