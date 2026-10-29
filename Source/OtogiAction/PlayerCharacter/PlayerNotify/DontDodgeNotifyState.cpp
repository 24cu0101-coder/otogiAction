// Fill out your copyright notice in the Description page of Project Settings.


#include "DontDodgeNotifyState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


void UDontDodgeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor))
		{
			// 範囲に入ったらタグを付与
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("IsDodge.DontDodge"));
		}
	}
}

void UDontDodgeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
		{
			// 範囲を出たらタグを削除
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("IsDodge.DontDodge"));
		}
	}
}
