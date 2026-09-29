// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CameraSequenceDataAsset.generated.h"

class ULevelSequence;


UCLASS()
class OTOGIACTION_API UCameraSequenceDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence")
	TObjectPtr<ULevelSequence> Sequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence")
	float BlendInTime = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence")
	float BlendOutTime = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence")
	bool bLockPlayerInput = true;
};
