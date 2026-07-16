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
	void ExcuteAreaAttack(float Width,float Height,float Length, FName TargetTag, float Damage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//多段ヒット防止用の配列
	UPROPERTY()
	TArray<AActor*> HitActors;

	//BoxCollisionの幅
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Box")
	float BoxWidth = 0.f;
	//BoxCollisionの奥行
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Box")
	float BoxLength = 0.f;
	//BoxCollisionの高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Box")
	float BoxHeight = 0.f;

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
