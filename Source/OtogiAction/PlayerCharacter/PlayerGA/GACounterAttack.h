// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GACounterAttack.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UGACounterAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
	UGACounterAttack();

public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle CounterAttack,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* CounterTriggerEvent
	) override;

protected:

	//通常攻撃のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CounterAttackMontages")
	UAnimMontage* CAttackMontage;

	//-------------------
	//関数
	//-------------------

	//アビリティシステム
	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* CAttackASC;


	UFUNCTION()
	void PlayCAttackMontage();



};
