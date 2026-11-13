// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/BaseAttackComponent.h"
#include "IaiAttackComponent.generated.h"

class UInputBufferComponent;


/**
 * 
 */
UCLASS()
class OTOGIACTION_API UIaiAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
protected:

	void AbilityEnd(const FAbilityEndedData& AbilityEndedData);

public:
	UIaiAttackComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	//プレイヤーのアクター
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;


	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IaiAttack.Ability")
	UAbilitySystemComponent* IaiAttackASC;

	//強攻撃アビリティ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IaiAttack.Gas")
	TSubclassOf<UGameplayAbility> IaiAttackAbility;

	//アビリティのハンドル
	FGameplayAbilitySpecHandle IaiHandle;

	//強攻撃アビリティを実行するs
	UFUNCTION()
	void ExecuteIaiAttackAbility();

	// chargedamageを渡す関数
	UFUNCTION()
	float GetChargeDamage() const { return m_chargeDamage; };

	UFUNCTION()
	// 
	void ChargeDamageReset();


private:
	

	UFUNCTION()

	void ExecuteIaiAttackAbility2();


	//キャラ移動コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IaiAttack.MovementComp", meta = (AllowPrivateAccess = "true"))
	UMoveComponent* MCC;

	//先行入力のコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IaiAttack.InputBufferComp", meta = (AllowPrivateAccess = "true"))
	UInputBufferComponent* IaiAttackInputBufferComp;

	bool SecondInputExecuted = false;

	void Iaidebug();

	// チャージ攻撃damageを決めます
	void ChargeDamage(float chargeduration);


	// チャージダメージ
	float m_chargeDamage;

	// チャージ開始時間
	float m_chargeStartTime;


};
