// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enemy/BossEnemyAttack01GA.h"
#include "BossEnemyAttack01Component.generated.h"

class UAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UBossEnemyAttack01Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossEnemyAttack01Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//設定するGAを選択する変数
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI|GAS")
	TArray<TSubclassOf<UGameplayAbility>> AttackAbility;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
