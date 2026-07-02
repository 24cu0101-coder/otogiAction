// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAPlayerDodge.generated.h"

/**
 * 
 */
//クラス前方宣言
class APlayerCharacter;		//プレイヤークラス

UCLASS()
class OTOGIACTION_API UGAPlayerDodge : public UGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Dodging,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;

	//プレイヤーの情報
	APlayerCharacter* PlayerActor;

protected:

	//回避の開始
	void DodgeStart();

	//回避の処理
	void IsDodge();

	//回避のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Montage")
	UAnimMontage* DodgeMontage;

private:

	//回避の数値
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DodgeParameter", meta = (AllowPrivateAccess = "true"))
	float DodgeDistance;
	
};
