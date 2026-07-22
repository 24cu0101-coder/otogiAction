// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatusComponent.generated.h"

//デリゲート宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamagedSignature, float, CurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadSignature);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UPlayerStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatusComponent();

	//受けたダメージを受け取ってHPを変更する
	UFUNCTION(BlueprintCallable, Category = "Status")
	float TakeDamage(float Damage);

	//体力を回復する関数
	UFUNCTION(BlueprintCallable, Category = "Status")
	void Heal(float HealAmount);

	//死んだことを外部に通知する関数
	UFUNCTION(BlueprintPure, Category = "Status")
	bool IsDead() const;

	//現在のHpを外部で受け取れるようにする関数
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetCurrentHP() const { return CurrentHP; }

	//最大HPを外部で受け取れる関数
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetMaxHP() const { return MaxHP; }

	//通知用イベント
	// UI通知用イベント
	UPROPERTY(BlueprintAssignable, Category = "Status|Events")
	FOnDamagedSignature OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Status|Events")
	FOnDeadSignature OnDead;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHP = 100.0f;

	//現在のHP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float CurrentHP = 100.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//プレイヤーが無敵か否か
	bool PlayerInvincible();
		
};
