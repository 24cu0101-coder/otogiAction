// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h" 
#include "InputBufferNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UInputBufferNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,UAnimSequenceBase* Animation,
		float TotalDuration,const FAnimNotifyEventReference& EventReference)override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerNotify.NextNAttack")
	FGameplayTag TypeAheadTag = FGameplayTag::RequestGameplayTag(FName("InputBufferReception"));

};
