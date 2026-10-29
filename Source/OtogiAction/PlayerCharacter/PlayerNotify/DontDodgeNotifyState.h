// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h" 
#include "DontDodgeNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UDontDodgeNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference)override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

//protected:
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerNotify. DontDodge")
//	FGameplayTag m_dontDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.DontDoge"));


};
