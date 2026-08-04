// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "AttackPressComponent.generated.h"

//攻撃開始時にCollisionComponentの関数を発火するdelegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FExecuteAttackPressDelegate, float, Radius, FName, TargetTag, float, Damage,
	float, StunPoint, float, HitStopDuration, float, HitStopTimeScale, float, ForwardOffset, float, SideOffset);

/**
 * 
 */
class UEnemyStateSubsystem;

UCLASS()
class OTOGIACTION_API UAttackPressComponent : public UEnemyAttackBaseComponent
{
	GENERATED_BODY()

public:
	UAttackPressComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//攻撃を開始する関数
	virtual void ExecuteAttack()override;

	//stateをセットする関数
	virtual void SetEnemyState()override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool StartAttackPress();

	bool IsAttacking() const { return bIsAttacking; }

	//攻撃開始を外部に伝えるためのハンドル
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FExecuteAttackPressDelegate StartAttackHandle;

	//攻撃半径のサイズを渡すための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Comp|Param")
	float Radius = 200.f;
	//与ダメージを渡すための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Comp|Param")
	float Damage = 20.f;
	//前にどれだけ出すかの値を渡すための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Comp|Param")
	float ForwardOffset = 80.f;
	//横にどれだけ出すかの値を渡すための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Comp|Param")
	float SideOffset = 0.f;

	float StunPoint = 0.f;			//使わない
	float HitStopDuration = 0.f;	//使わない
	float HitStopTimeScale = 0.f;	//使わない

	//攻撃対象を渡すための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Comp|Param")
	FName TargetTag = "";

protected:
	TWeakObjectPtr<UAnimInstance> CachedAnimInstance;
	bool bIsAttacking = false;

	
};
