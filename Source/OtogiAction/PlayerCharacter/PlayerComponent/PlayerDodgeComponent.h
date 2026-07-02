// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "PlayerDodgeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UPlayerDodgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerDodgeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	//回避のアビリティ
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "DodgeGas")
	TSubclassOf<UGameplayAbility> DodgeAbility;


	//アビリティのハンドル
	FGameplayAbilitySpecHandle DodgeHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//初期化時に呼ばれるabilityを付与する関数
	void DodgeAbilitySet(UAbilitySystemComponent* DodgeASC);
		
};

