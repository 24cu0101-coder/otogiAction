// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/BaseAttackComponent.h"
#include "StrongAttackComponent3.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UStrongAttackComponent3 : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:


	////通常攻撃のアビリティ
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SlAttack2.Gas")
	//TSubclassOf<UGameplayAbility> SAttackAbility2;

	////アビリティのハンドル
	//FGameplayAbilitySpecHandle NAttackHandle;

public:
	
	//次元斬・絶を実行する処理
	void ExecuteSutrongAttack3Ability();



};
