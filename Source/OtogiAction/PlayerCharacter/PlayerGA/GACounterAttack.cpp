// Fill out your copyright notice in the Description page of Project Settings.


#include "GACounterAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGACounterAttack::UGACounterAttack()
{

}


void UGACounterAttack::ActivateAbility(const FGameplayAbilitySpecHandle CounterAttack, const FGameplayAbilityActorInfo* playerActorInfo, const FGameplayAbilityActivationInfo AvtivationInfo, const FGameplayEventData* CounterTriggerEvent)
{
	Super::ActivateAbility(CounterAttack, playerActorInfo, AvtivationInfo, CounterTriggerEvent);

	//アビリティシステムコンポーネントを取得
	CAttackASC = GetAbilitySystemComponentFromActorInfo();

	//アビリティコンポーネントがあれば
	if (CAttackASC)
	{
		PlayCAttackMontage();
	}

	else
	{
		return;
	}

}

//カウンター攻撃アニメーション再生
void UGACounterAttack::PlayCAttackMontage()
{
	//if (CAttackMontage)
	//{
	//	UAbilityTask_PlayMontageAndWait 

	//}
}


