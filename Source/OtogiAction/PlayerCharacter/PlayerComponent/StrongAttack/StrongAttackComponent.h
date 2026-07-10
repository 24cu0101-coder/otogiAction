// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "StrongAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UStrongAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStrongAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//プレイヤーのアクター
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;


	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SAttack.Ability")
	UAbilitySystemComponent* SAttackASC;

	//強攻撃アビリティ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAttack.Gas")
	TSubclassOf<UGameplayAbility> SAttackAbility;

	//アビリティのハンドル
	FGameplayAbilitySpecHandle DodgeHandle;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//強攻撃
	void ExecuteStrongAttackAbility();
		
};
