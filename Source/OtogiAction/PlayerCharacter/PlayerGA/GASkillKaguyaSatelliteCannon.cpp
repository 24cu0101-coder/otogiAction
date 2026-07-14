// Fill out your copyright notice in the Description page of Project Settings.


#include "GASkillKaguyaSatelliteCannon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"



UGASkillKaguyaSatelliteCannon::UGASkillKaguyaSatelliteCannon()
{

}

void UGASkillKaguyaSatelliteCannon::ActivateAbility(
	const FGameplayAbilitySpecHandle NormalAttack,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
)
{
	Super::ActivateAbility(NormalAttack, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	KSAsc = GetAbilitySystemComponentFromActorInfo();

	//世界破壊光線発射
	WorldDestruction();
}

//敵をせん滅する
void UGASkillKaguyaSatelliteCannon::WorldDestruction()
{
	PlayNSPMontage();
}

//アニメーション再生関数
void UGASkillKaguyaSatelliteCannon::PlayNSPMontage()
{
	//衛星兵器起動指示のアニメーションモンタージュがあれば
	if (KSPMontage)
	{
		//アニメーション再生タスク
		UAbilityTask_PlayMontageAndWait* KAPMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
			(this, NAME_None, KSPMontage);

		//再生のタスクがあれば
		if (KAPMontageTask)
		{
			//終了時に呼ぶ
			KAPMontageTask->OnInterrupted.AddDynamic(this, &UGASkillKaguyaSatelliteCannon::KaguyaSatelliteEnd);
		}
	}
}


//
void UGASkillKaguyaSatelliteCannon::KaguyaSatelliteEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
