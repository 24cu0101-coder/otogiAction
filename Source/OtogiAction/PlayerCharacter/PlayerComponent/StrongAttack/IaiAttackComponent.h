// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/BaseAttackComponent.h"
#include "IaiAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UIaiAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
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

	//強攻撃アビリティを実行する
	void ExecuteIaiAttackAbility();


private:
	//キャラ移動コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IaiAttack.MovementComp", meta = (AllowPrivateAccess = "true"))
	UMoveComponent* MCC;



};
