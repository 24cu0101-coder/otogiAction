// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackBaseComponent.generated.h"

//攻撃開始時に発火するdelegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExecuteAttackDelegate);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UEnemyAttackBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyAttackBaseComponent();

	//攻撃を開始する関数
	UFUNCTION(Blueprintcallable,Category = "Attack")
	virtual void ExecuteAttack();

	//攻撃が終了したときに呼び出す関数
	UFUNCTION(Blueprintcallable, Category = "Attack")
	void FinishAttack(bool bSuccess);

	//攻撃開始を外部に伝えるためのハンドル
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FExecuteAttackDelegate StartAttackHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 攻撃で使用するアニメーションモンタージュなどを共通データとして持てる
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Visual")
	TObjectPtr<UAnimMontage> AttackMontage;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
