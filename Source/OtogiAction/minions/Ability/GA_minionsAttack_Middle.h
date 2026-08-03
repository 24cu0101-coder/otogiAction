// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Animation/AnimMontage.h"
#include "NiagaraSystem.h"

#include "GA_minionsAttack_Middle.generated.h"

class UAbilityTask_PlayMontageAndWait;

UCLASS()
class OTOGIACTION_API UGA_minionsAttack_Middle : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_minionsAttack_Middle();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;


protected:

	// 攻撃Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;


	// ダメージ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage = 20.f;


	// Montage終了時
	UFUNCTION()
	void OnMontageCompleted();

	//エフェクト
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	UNiagaraSystem* AttackEffect = nullptr;

	UFUNCTION()
	void OnMontageInterrupted();


private:

	FGameplayAbilitySpecHandle CurrentSpecHandle;
	const FGameplayAbilityActorInfo* CurrentActorInfo;
	FGameplayAbilityActivationInfo CurrentActivationInfo;
};