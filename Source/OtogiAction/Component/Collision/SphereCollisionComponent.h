// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SphereCollisionComponent.generated.h"

class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API USphereCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USphereCollisionComponent();

	//
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ExcuteAreaAttack(float Radius, FName TargetTag, float Damage, float HitStopDuration, float HitStopTimeScale);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//多段ヒット防止用の配列
	UPROPERTY()
	TArray<AActor*> HitActors;

	//SphereCollisionの半径
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Sphere")
	float SphereRadius = 0.f;

	//コリジョンをどれだけ前に出すか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float ForwardOffset = 0.f;

	//コリジョンをどれだけ横に出すか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float SideOffset = 0.f;

	//与えるダメージの変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float GiveDamage = 0.f;

	//コリジョンの当たり判定の計算
	FCollisionShape HitCollisionShape;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
