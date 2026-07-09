// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
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

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void AbilityFinished();

	//召喚する斧のアクタークラス
	UPROPERTY(EditDefaultsOnly, Category = "Kintaro")
	TSubclassOf<AActor>AxActorClass;

	//どの位置に出すか
	UPROPERTY(EditDefaultsOnly, Category = "Kintaro")
	FVector SpawnOffset = FVector(150.f, 0.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Kintaro")
	UAnimMontage* KintaroMontage;
	
private:
	UPROPERTY()
	AActor* SpawnAxActor;

};
