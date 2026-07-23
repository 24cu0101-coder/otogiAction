// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/BaseAttackComponent.h"
#include "GASkillUrasima.generated.h"

class FGameplayEffectRemoveInfo;

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UGASkillUrasima : public UGameplayAbility
{
	GENERATED_BODY()

	UGASkillUrasima();

public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle TheWorld,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;



	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	)override;

protected:

	//箱を開けるモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TamatebakoMontage")
	UAnimMontage* TamatebakoMontages;

	//プレイヤーのアクター
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Urasima.Effects")
	TSubclassOf<UGameplayEffect> TimeStopEffect;

	FActiveGameplayEffectHandle ActiveEffectHandle;

	UFUNCTION()
	//アクター全てをscanする
	void ScanActor();

	UFUNCTION()
	//時は動き出す
	void TimeRestart();

	UFUNCTION()
	//GE解除時に呼ばれる関数
	void TheWorldDelegate(const FGameplayEffectRemovalInfo& RemovalInfo);

private:
	//停止する時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TimeStopParameter", meta = (AllowPrivateAccess = "true"))
	float StopTime;

	//自分のアクター
	AActor* OwnerActor;

	//プレイヤー野コントローラー
	AController* OwnerController;


	//全てのアクターを格納するアクタ－の配列
	TArray<AActor*> FoundActors;

	//アビリティシステムコンポーネント
	UAbilitySystemComponent* UrasimaASC;

};
