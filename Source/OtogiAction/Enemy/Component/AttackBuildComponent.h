// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "AttackBuildComponent.generated.h"

//攻撃開始時にCollisionComponentの関数を発火するdelegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_EightParams(FExecuteAttackBuildDelegate, float, Radius, FName, TargetTag, float, Damage,
	float, StunPoint, float, HitStopDuration, float, HitStopTimeScale, float, ForwardOffset, float, SideOffset);

/**
 *
 */
class UEnemyStateSubsystem;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UAttackBuildComponent : public UEnemyAttackBaseComponent
{
	GENERATED_BODY()

public:
	UAttackBuildComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//攻撃を開始する関数
	virtual void ExecuteAttack()override;

	//stateをセットする関数
	virtual void SetEnemyState()override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool StartAttackBuild();

	bool IsAttacking() const { return bIsAttacking; }

	//攻撃開始を外部に伝えるためのハンドル
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FExecuteAttackBuildDelegate StartAttackHandle;

	//攻撃半径を渡すための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Comp|Param")
	float Radius = 250.f;
	//与ダメージを渡すための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Comp|Param")
	float Damage = 50.f;
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
