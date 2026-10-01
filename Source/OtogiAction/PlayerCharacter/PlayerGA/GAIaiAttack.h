// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAIaiAttack.generated.h"

//クラス前方宣言
class APlayerCharacter;
class UPlayerTargetComponent;

/**
 *
 */
UCLASS()
class OTOGIACTION_API UGAIaiAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGAIaiAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle IaiAttack,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;

protected:

	//居合攻撃のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* m_iaiAttackMontage;

	//納刀アニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* m_sheathingMontage;


	//プレイヤーの情報
	UPROPERTY(Transient)
	APlayerCharacter* m_playerActor;

	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* m_ASC;


	// 納刀
	UFUNCTION()
	void SheathingSword();

	// 居合攻撃
	UFUNCTION()
	void IaiSlash();

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
	void Rotate(FVector TargetLocation);

	UFUNCTION()
	void Sheathing(FGameplayEventData Payload);


	UFUNCTION()
	//居合攻撃専用のワーピング処理
	void IaiWarping();

	UPROPERTY()
	UPlayerTargetComponent* m_playerTargetComp;


private:

	FTimerHandle m_iaiTimer;
	
	//キャラクターのアクター変数
	ACharacter* m_char;

	//stepの距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IaiAttackParameter", meta = (AllowPrivateAccess = "true"))
	float m_iaiDistance;

	//ステップする時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IaiAttackParameter", meta = (AllowPrivateAccess = "true"))
	float m_iaiTime;

	//ステップのディレイ時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IaiAttackParameter", meta = (AllowPrivateAccess = "true"))
	float m_iaiDelayTiem;

	UPROPERTY()
	AActor* m_warpTargetActor = nullptr;


	//このコンポーネントの持ち主
	UPROPERTY()
	ACharacter* m_ownerCharacter;

	UFUNCTION()
	void PlayerVisible(bool Visible);

	UFUNCTION()
	void IaiVisible();

	UFUNCTION()
	void StopMontage();

	bool m_iaiStance = false;

};
