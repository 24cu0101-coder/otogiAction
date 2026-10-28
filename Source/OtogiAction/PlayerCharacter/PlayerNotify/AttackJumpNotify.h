// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackJumpNotify.generated.h"

class UNormalComboAttackComponent;

UCLASS()
class OTOGIACTION_API UAttackJumpNotify : public UAnimNotify
{
	GENERATED_BODY()

	// ’Ê’m‚Ìˆ—
	virtual void Notify(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference)
		override;

	// ƒWƒƒƒ“ƒv‚Ì’µ–ô‚Ì‚½‚©‚³‚¤‚¨‚¨‚¨‚¨
	UPROPERTY(EditAnywhere, Category = "Attack MoveParameter")
	float UpPower;

	// UŒ‚‚ÌˆÚ“®—Ê‚¸‚ ‚ ‚ ‚ 
	UPROPERTY(EditAnywhere, Category = "Attack MoveParameter")
	float forwardPower;
};
