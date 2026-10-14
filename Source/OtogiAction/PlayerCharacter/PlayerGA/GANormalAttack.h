// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GANormalAttack.generated.h"


//クラス前方宣言
class APlayerCharacter;		//プレイヤークラス
class UAbilityTask_ApplyRootMotionConstantForce;
/**
 * 
 */
UCLASS()
class OTOGIACTION_API UGANormalAttack : public UGameplayAbility
{
	GENERATED_BODY()

	UGANormalAttack();

public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle NormalAttack,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;


protected:

	//通常攻撃のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NAttackMontages")
	UAnimMontage* NAttackMontages;

	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	//-------------------
	//関数
	//-------------------

	UFUNCTION()
	void PlayNAttackMontage();


	//loopをcountする
	UFUNCTION()
	void LoopCount(FGameplayEventData Payload);

	////slowにするタスクを設定する
	//UFUNCTION()
	//void SloawMontageTaskSet();

	UFUNCTION()
	void SloawMontage(FGameplayEventData Payload);

	UFUNCTION()
	void EventTaskEnd(UAbilityTask_WaitGameplayEvent* EventTask);

	//攻撃時前進する関数
	UFUNCTION()
	void AttackMove();

	//アニメーション終了時に呼ばれるタスク
	UFUNCTION()

	void NAttackMontageEnd();
	
	//アビリティ終了の関数
	UFUNCTION()

	void NAttackAbilityEnd();



	////次に再生するアニメーションのタグを渡す関数
	//void NextNAttack(FGameplayEventData EventData);

	 
	//-------------------


	//-------------------
	//変数
	//-------------------

	//再生するモンタージュの配列番号
	int32 NAttackIndex;

	//アビリティシステム
	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* NAttackASC;



	//-------------------



	////攻撃実行中のタグ
	//FGameplayTag IsNormalAttackTag;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.AttackMoveDistance", meta = (AllowPrivateAccess = "true"))
	float AttackMoveDistance;


	int32 Count;
	int32 Countlimit;

	UAbilityTask_PlayMontageAndWait* NAttackMontageTask;
	UAbilityTask_WaitGameplayEvent* SheathingEvent;
	UAbilityTask_WaitGameplayEvent* SlowMontageEvent;

};
