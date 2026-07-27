// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h" 
#include "MontageRoopNotify.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UMontageRoopNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	// 通知の処理
	virtual void Notify(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference)
		override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerNotify.LoopOut")
	FGameplayTag LoopOutTag;





};
