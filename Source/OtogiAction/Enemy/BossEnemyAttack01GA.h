// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BossEnemyAttack01GA.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UBossEnemyAttack01GA : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void TriggerBossAttack();

	TArray<TSubclassOf<UGameplayAbility>> AttackAbility;
};
