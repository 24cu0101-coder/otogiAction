// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAIaiAttack.generated.h"

//クラス前方宣言
class APlayerCharacter;


/**
 * 
 */
UCLASS()
class OTOGIACTION_API UGAIaiAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle IaiAttack,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;

protected:

	//居合攻撃のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* IaiAttackMontage;

	//納刀アニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* SheathingMontage;


	//プレイヤーの情報
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* ASC;


	//モンタージュを再生する関数
	UFUNCTION()
	void PlayIaiAttackMontage();

	UFUNCTION()
	void RestartIaiAttackMontage();


	UFUNCTION()
	void IaiAttackMontageEnd();

	UFUNCTION()
	void IaiAttackAbilityEnd();

	UFUNCTION()
	void RestartMontage();




	UFUNCTION()
	void Iaistep();

	UFUNCTION()
	void Rotate();

	UFUNCTION()
	void Sheathing(FGameplayEventData Payload);


private:

	FTimerHandle IaiTimer;


	//stepの距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IaiAttackParameter", meta = (AllowPrivateAccess = "true"))
	float IaiDistance;

	//ステップする時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IaiAttackParameter", meta = (AllowPrivateAccess = "true"))
	float IaiTime;

	//ステップのディレイ時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IaiAttackParameter", meta = (AllowPrivateAccess = "true"))
	float IaiDelayTiem;



};
