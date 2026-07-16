// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitStopComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UHitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitStopComponent();

	//ヒットストップ開始
	UFUNCTION(BlueprintCallable, Category = "Combat | HitStop")
	void TriggerHitStop(AActor* Attacker, AActor* Victim, float Duration = 0.1f, float TimeScale = 0.05f);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//ヒットストップタイマー
	FTimerHandle HitStopTimerHandle;

	//あたった相手を保存
	UPROPERTY()
	AActor* SavedAttacker;

	UPROPERTY()
	AActor* SavedVictim;

	void ResetHitStop();

		
};
