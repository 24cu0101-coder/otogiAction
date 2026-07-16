// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_KintaroAttackAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

//コンストラクタ
UGA_KintaroAttackAbility::UGA_KintaroAttackAbility()
{

}

//abilityの発動関数
void UGA_KintaroAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (KintaroAttackMontage)
	{
		//モンタージュタスク作成
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,TEXT("KintaroAttackAction"),KintaroAttackMontage,1.0f);

		if (PlayMontageTask)
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_KintaroAttackAbility::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_KintaroAttackAbility::OnMontageFinished);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_KintaroAttackAbility::OnMontageFinished);

			PlayMontageTask->ReadyForActivation();
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

//Abilityの終了
void UGA_KintaroAttackAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
