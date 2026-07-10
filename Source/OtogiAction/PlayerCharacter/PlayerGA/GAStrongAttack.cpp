// Fill out your copyright notice in the Description page of Project Settings.


#include "GAStrongAttack.h"

void UGAStrongAttack::ActivateAbility(const FGameplayAbilitySpecHandle SAttack,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent)
{
	Super::ActivateAbility(SAttack, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティ取得
	ASC = GetAbilitySystemComponentFromActorInfo();

	//プレイヤーの情報を取得
	PlayerActor = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	if (!PlayerActor || SAttackMontage)
	{
		//リターン
		return;
	}		
}