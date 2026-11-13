// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "StateClass.h"

#include "ChaseDecorator.generated.h"

/**
 * 
 */

UCLASS()
class OTOGIACTION_API UChaseDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UChaseDecorator();

protected:
	//条件判定の本体（trueならノード通過、falseならブロック）
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	// Blackboardの値が変化した時に再評価するためのコールバック
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;

	// エディタ上で比較対象として指定するState
	UPROPERTY(EditAnywhere, Category = "Condition")
	EEnemyState RequiredState = EEnemyState::Idle;
};
