// Fill out your copyright notice in the Description page of Project Settings.


#include "JustDodgeNotifyState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/NormalAttack/NormalComboAttackComponent.h"



void UJustDodgeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
		{
			// îÕàÕÇ…ì¸Ç¡ÇΩÇÁÉ^ÉOÇïtó^
			ASC->AddLooseGameplayTag(m_justDodgeTag);
		}
	}
}

void UJustDodgeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
		{
			// îÕàÕÇ…ì¸Ç¡ÇΩÇÁÉ^ÉOÇïtó^
			ASC->RemoveLooseGameplayTag(m_justDodgeTag);

			UE_LOG(LogTemp, Warning, TEXT("ÅyGAPlayerDodge.cppÅz: ooooooooooo"));

		}
	}
}
