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

	UGAPlayerDodge();

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

	//回避のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* DodgeMontage;

	//-------------------
	//関数
	//-------------------


	//回避の開始
	UFUNCTION()

	void DodgeStart();

	//回避の処理
	UFUNCTION()

	void IsDodge();

	//回避終了時の処理
	UFUNCTION()

	void DodgeEnd();

	//モンタージュ終了
	UFUNCTION()

	void DodgeMontageEnd();


	//-------------------
	//-------------------


	//回避実行中のタグ
	FGameplayTag IsDodgeTag;

private:

	//回避の数値
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DodgeParameter", meta = (AllowPrivateAccess = "true"))
	float DodgeDistance;

	//回避する時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DodgeParameter", meta = (AllowPrivateAccess = "true"))
	float DodgeTime;

	//回避のディレイの時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DodgeParameter", meta = (AllowPrivateAccess = "true"))
	float DelayTiem;

	//回避時間
	FTimerHandle DodgeTimer;



	//アビリティシステム
	UAbilitySystemComponent* ASC;
	
};
