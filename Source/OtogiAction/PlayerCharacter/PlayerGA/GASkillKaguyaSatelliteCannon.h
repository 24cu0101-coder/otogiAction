// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASkillKaguyaSatelliteCannon.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UGASkillKaguyaSatelliteCannon : public UGameplayAbility
{
	GENERATED_BODY()

	UGASkillKaguyaSatelliteCannon();
	
public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle NormalAttack,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;

	
protected:

	//通常攻撃のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KaguyaSatellitePointMontage")
	UAnimMontage* KSPMontage;

	//衛星兵器起動指示
	UFUNCTION()
	void PlayNSPMontage();

	//衛星兵器で敵を殲滅する関数
	UFUNCTION()
	void WorldDestruction();

	UFUNCTION()
	void KaguyaSatelliteEnd();

	//アビリティシステム
	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* KSAsc;

};
