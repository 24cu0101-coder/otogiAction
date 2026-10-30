// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAPlayerJustDodge.generated.h"

/**
 * 
 */

class APlayerCharacter;		//プレイヤークラス
class UDamageType;
class UAbilityTask_WaitGameplayTagRemoved;
class UAbilityTask_ApplyRootMotionConstantForce;


UCLASS()
class OTOGIACTION_API UGAPlayerJustDodge : public UGameplayAbility
{
	GENERATED_BODY()
	
	//UGAPlayerJustDodge();

public:

	/*virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Dodging,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;*/


protected:

	////ジャスト回避のアニメーションモンタージュ
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	//UAnimMontage* JustDodgeMontage;


	////プレイヤーの情報
	//UPROPERTY(Transient)
	//APlayerCharacter* m_playerActor;



	//-------------------
    //関数
    //-------------------






};
