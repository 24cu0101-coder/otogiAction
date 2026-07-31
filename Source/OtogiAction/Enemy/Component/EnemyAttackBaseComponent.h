// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EnemyAttackBaseComponent.generated.h"

class AAIController;
class APawn;

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
	virtual void FinishAttack(bool bSuccess);

	//行動の評価値を計算して返す関数
	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	virtual float CalculateScore(AAIController* Controller, APawn* ControlledPawn);

	//stateをセットする関数
	virtual void SetEnemyState();

	//ABPでのアニメーション変更用変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard", meta = (UseByRequest = "true"))
	FBlackboardKeySelector CanAttackBuildKey;

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
