// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASkillKintaro.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UGASkillKintaro : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGASkillKintaro();

	// アビリティ開始時の処理
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
protected:
	//変身モンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kintaro | Visual")
	UAnimMontage* KintaroMontage;

	//金太郎状態GameplayEffect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kintaro | Effects")
	TSubclassOf<UGameplayEffect> KintaroEffectClass;

private:
	// アニメーションが終了（またはキャンセル）した時のコールバック関数
	UFUNCTION()
	void AbilityFinished();
};
