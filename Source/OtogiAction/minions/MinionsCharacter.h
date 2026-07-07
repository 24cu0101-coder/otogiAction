// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "MinionsCharacter.generated.h"

class UStatusComponent;
class UminionsAttackComponent;
UCLASS()
class OTOGIACTION_API AMinionsCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AMinionsCharacter();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// スキル登録
	void GiveDefaultAbilities();

	//ステータス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UminionsAttackComponent* AttackComponent;

	//攻撃コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStatusComponent* StatusComponent;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
