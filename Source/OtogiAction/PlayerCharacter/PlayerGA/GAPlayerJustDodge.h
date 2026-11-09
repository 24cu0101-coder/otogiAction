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
class UPlayerDodgeComponent;

UCLASS()
class OTOGIACTION_API UGAPlayerJustDodge : public UGameplayAbility
{
	GENERATED_BODY()
	
	UGAPlayerJustDodge();

public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Dodging,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;


protected:

	//ジャスト回避のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* JustDodgeMontage;

	//アビリティシステム
	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* m_asc;

	//プレイヤーの情報
	UPROPERTY(Transient)
	APlayerCharacter* m_playerActor;

	UPlayerDodgeComponent* m_playerDogeComp;

	//-------------------
    //関数
    //-------------------



private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.JustDodge", meta = (AllowPrivateAccess = "true"))
	float JustDodgeDistance;

	//just回避時のスロー倍率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.JustDodge", meta = (AllowPrivateAccess = "true"))
	float SlowMagnification;

	//ジャスト回避のスローのフレーム
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.JustDodge", meta = (AllowPrivateAccess = "true"))
	float SlowTime;


	// ジャスト回避のタグ
	FGameplayTag m_justDodge;

	FGameplayTag m_justDogeAttack;

	void PlayJustDodge();

	void JustDodgeLocation();

	void JustDodgeRotate(FVector Angle);

	UFUNCTION()
	void EndJustDodge();

	UFUNCTION()
	void JustDodgeMontageEnd();

};
