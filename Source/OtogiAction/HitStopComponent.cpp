// Fill out your copyright notice in the Description page of Project Settings.


#include "HitStopComponent.h"

// Sets default values for this component's properties
UHitStopComponent::UHitStopComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UHitStopComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHitStopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHitStopComponent::TriggerHitStop(AActor* Attacker, AActor* Victim, float Duration, float TimeScale)
{

	if (!Attacker || !Victim) return;

	// すでに実行中ならタイマーをリセットして上書き
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HitStopTimerHandle);
	}

	ResetHitStop();

	SavedAttacker = Attacker;
	SavedVictim = Victim;

	// スロー開始
	SavedAttacker->CustomTimeDilation = TimeScale;
	SavedVictim->CustomTimeDilation = TimeScale;

	// 元に戻すタイマーを実時間でセット
	if (GetWorld())
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UHitStopComponent::ResetHitStop);
		GetWorld()->GetTimerManager().SetTimer(HitStopTimerHandle, TimerDel, Duration, false);
	}
}



void UHitStopComponent::ResetHitStop()
{

	if (SavedAttacker && SavedAttacker->IsValidLowLevel())
	{
		SavedAttacker->CustomTimeDilation = 1.0f;
	}
	if (SavedVictim && SavedVictim->IsValidLowLevel())
	{
		SavedVictim->CustomTimeDilation = 1.0f;
	}

	SavedAttacker = nullptr;
	SavedVictim = nullptr;
}

