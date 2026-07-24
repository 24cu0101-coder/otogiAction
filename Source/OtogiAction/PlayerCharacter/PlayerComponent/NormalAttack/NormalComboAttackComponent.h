// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/BaseAttackComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "NormalComboAttackComponent.generated.h"

//前方宣言
class APlayerCharacter;
class UMoveComponent;
class UAttackCollisionComponent;


/**
 * 
 */
UCLASS()
class OTOGIACTION_API UNormalComboAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	UNormalComboAttackComponent();

protected:
	//プレイヤーのアクター
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;


	// Called when the game starts
	virtual void BeginPlay() override;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NormalAttack.Ability")
	UAbilitySystemComponent* AbilitySystemComponent;

	//通常攻撃のアビリティ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NormalAttack.Gas")
	TSubclassOf<UGameplayAbility> NAttackAbility1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NormalAttack.Gas")
	TSubclassOf<UGameplayAbility> NAttackAbility2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NormalAttack.Gas")
	TSubclassOf<UGameplayAbility> NAttackAbility3;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NormalAttack.Gas")
	TSubclassOf<UGameplayAbility> NAttackAbility4;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NormalAttack.Gas")
	TSubclassOf<UGameplayAbility> NAttackAbilityFinish;


	//通常攻撃のアビリティ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KintaroAttack.Gas")
	TSubclassOf<UGameplayAbility> KintaroAttack1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KintaroAttack.Gas")
	TSubclassOf<UGameplayAbility> KintaroAttack2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KintaroAttack.Gas")
	TSubclassOf<UGameplayAbility> KintaroAttack3;

	//アビリティのハンドル
	FGameplayAbilitySpecHandle NAttackHandle;



public:
	// Called every frame

	//通常攻撃アビリティ起動関数
	void ExecuteNormalAttackAbility();

	//ノーティファイにASCを渡す関数(多分辞めた方がいい)
	UAbilitySystemComponent* GetNAttackACS()const { return AbilitySystemComponent; }


private:
	//キャラ移動コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NAttack.MovementComp", meta = (AllowPrivateAccess = "true"))
	UMoveComponent* MCC;





};
