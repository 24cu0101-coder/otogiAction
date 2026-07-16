// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "JumpAttackDecorator.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UJumpAttackDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UJumpAttackDecorator();

protected:
	//条件を満たしているかどうか
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetTimeKey; // サービスで設定した目標時刻キー
};
