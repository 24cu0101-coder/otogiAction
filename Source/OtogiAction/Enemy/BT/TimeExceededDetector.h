// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "TimeExceededDetector.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UTimeExceededDetector : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UTimeExceededDetector();

protected:
	//条件を満たしているか判別する関数
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetTimeKey; // サービスで設定した目標時刻キー
};
