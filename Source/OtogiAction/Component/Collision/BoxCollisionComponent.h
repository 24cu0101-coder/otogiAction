// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoxCollisionComponent.generated.h"

class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UBoxCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBoxCollisionComponent();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ExcuteAreaAttack(float Width,float Height,float Length, FName TargetTag, float Damage,
		float ForwardOffset,float SideOffset);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//多段ヒット防止用の配列
	UPROPERTY()
	TArray<AActor*> HitActors;


	//コリジョンの当たり判定の計算
	FCollisionShape HitCollisionShape;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
