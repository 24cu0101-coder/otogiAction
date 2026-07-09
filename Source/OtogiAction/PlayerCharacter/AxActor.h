// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AxActor.generated.h"

UCLASS()
class OTOGIACTION_API AAxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// 振り下ろしにかかる合計時間
	float TotalSwingTime = 0.5f;
	// 経過時間カウント
	float ElapsedTime = 0.0f;

	FVector StartLocation;
	FRotator StartRotation;
	FVector ForwardVector;

};
