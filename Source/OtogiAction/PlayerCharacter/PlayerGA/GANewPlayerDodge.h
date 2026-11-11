// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GANewPlayerDodge.generated.h"

 //クラス前方宣言
class APlayerCharacter;		//プレイヤークラス
class UDamageType;
class UAbilityTask_WaitGameplayTagRemoved;
class UAbilityTask_ApplyRootMotionConstantForce;

UCLASS()
class OTOGIACTION_API UGANewPlayerDodge : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Dodging,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;



protected:

	//回避のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* DodgeMontage;

	//ジャスト回避のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* JustDodgeMontage;


	//プレイヤーの情報
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	//-------------------
	//関数
	//-------------------


	//回避の開始
	UFUNCTION()

	void DodgeStart();

	//回避時の移動関数
	UFUNCTION()

	void DodgeLocation(float Distance);

	// 回避の移動を開始
	UFUNCTION()
	void DodgeLocationStart();

	//回避終了時の関数
	UFUNCTION()

	void DodgeEnd();

	//モンタージュ終了
	UFUNCTION()

	void DodgeMontageEnd();

	

	//回避開始の関数
	UFUNCTION()
	void PlayDodge();



	//-------------------
	//-------------------


	//回避実行中のタグ
	FGameplayTag IsDodgeTag;

	//無敵中のタグ
	FGameplayTag IsInvincibleTag;

	// just回避中のタグ
	FGameplayTag m_justDodgeTag;

	//カウンター攻撃のタグ
	FGameplayTag IsCounterAttackTag;

	//アビリティシステム
	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* ASC;

	// 実行中のタスクを保持する変数
	UPROPERTY()
	TObjectPtr<UAbilityTask_ApplyRootMotionConstantForce> m_moveTask;

private:

	//回避の距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.NormalDodge", meta = (AllowPrivateAccess = "true"))
	float DodgeDistance;

	//回避する時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.NormalDodge", meta = (AllowPrivateAccess = "true"))
	float DodgeTime;

	//回避のディレイの時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.NormalDodge", meta = (AllowPrivateAccess = "true"))
	float DelayTiem;

	//回避時の無敵の時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.JustDodge", meta = (AllowPrivateAccess = "true"))
	float InvincibleTime;

	UPROPERTY(EditDefaultsOnly,Category = "Parameter.DodgeSpeedCurve")
	UCurveFloat* DedgeSpeedCurve;


private:


	//現在無敵かどうか
	bool IsInvincibleFlag = false;

	//回避時間
	FTimerHandle DodgeTimer;

	//数秒後処理終了
	FTimerHandle EndDodgeTimer;

	//数秒後処理終了
	FTimerHandle m_endJustDodgeTimer;

	// ジャスト回避後のプレイヤーの向き
	FVector m_justDodgeRelativeAngle;

	FGameplayTag m_justDodgeAttack;

	// 攻撃された敵のアクター
	AActor* m_enemyActor;

	UFUNCTION()
	void StickRotate();


	//移動の関数をバインドするでりげーど変数 
	FTimerDelegate LocationDelegate;



};
