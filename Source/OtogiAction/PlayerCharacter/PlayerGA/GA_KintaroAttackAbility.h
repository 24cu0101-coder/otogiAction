// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_KintaroAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UGA_KintaroAttackAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_KintaroAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	//çƒê∂Ç∑ÇÈçUåÇÉÇÉìÉ^Å[ÉWÉÖ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kintaro")
	UAnimMontage* KintaroAttackMontage;

private:
	UFUNCTION()
	void OnMontageFinished();
	
};
