// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h" 
#include "JustDodgeNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UJustDodgeNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference)override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;


protected:
	FGameplayTag m_justDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.JustDodge"));


};
