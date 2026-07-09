// Fill out your copyright notice in the Description page of Project Settings.

#include "GANormalAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"

//コンストラクタ
UGANormalAttack::UGANormalAttack():
	NAttackIndex(0),
	NAttackASC(nullptr)
{

}


void UGANormalAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle NormalAttack,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
) 
{
	Super::ActivateAbility(NormalAttack, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	NAttackASC = GetAbilitySystemComponentFromActorInfo();
			
	//アニメーションを再生する関数を呼ぶ
	PlayNAttackMontage();
}

//アニメーション再生
void UGANormalAttack::PlayNAttackMontage()
{

	if (NAttackMontages)
	{

		//アニメーション再生タスク
		UAbilityTask_PlayMontageAndWait* NAttackMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
			(this, NAME_None, NAttackMontages);

		//タスクがあれば
		if (NAttackMontageTask)
		{
			//終了時に自動で呼ぶ
			if (!NAttackMontageTask->OnCancelled.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnCancelled.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
			if (!NAttackMontageTask->OnCompleted.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnCompleted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
			if (!NAttackMontageTask->OnInterrupted.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnInterrupted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
		}

		//アニメーション再生
		NAttackMontageTask->ReadyForActivation();

	}
}

//モンタージュ終了時に呼び出す
void UGANormalAttack::NAttackMontageEnd()
{

	NAttackAbilityEnd();
}

//アビリティ終了を終了する
void UGANormalAttack::NAttackAbilityEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}