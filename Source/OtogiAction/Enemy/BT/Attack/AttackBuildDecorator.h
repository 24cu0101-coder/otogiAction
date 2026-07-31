// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AttackBuildDecorator.generated.h"

/**
 * 
 */

class UEnemyStateSubsystem;

UCLASS()
class OTOGIACTION_API UAttackBuildDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UAttackBuildDecorator();

protected:
	//条件のTrue,Falseを判定する関数
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	//デバッグ用
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DebugKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool DebugKeyFlg = false;
};
