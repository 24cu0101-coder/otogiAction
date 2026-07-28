// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "TypeAheadComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UTypeAheadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTypeAheadComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	bool TypeAhead();

	//プレイヤーのアクター
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InputBuffer.Ability")
	UAbilitySystemComponent* AbilitySystemComponent;

	void vss(const FGameplayEventData* Payload);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//private:
//
	//各アクションの実行する関数を保存する変数
	void GetBufferedAction(TFunction<void()> Function);

	TFunction<void()> SetBufferedAction();

	TFunction<void()> ExeFunction;

};
