// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MoveStopedNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UMoveStopedNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:

	// バーが始まったらロックON
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	// バーが終わったらロックOFF
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
