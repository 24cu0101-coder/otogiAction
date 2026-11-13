// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include  "StateClass.h"

#include "BossStrafeDecorator.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UBossStrafeDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBossStrafeDecorator();

protected:
	// 1. 条件判定（trueならStrafeTaskを実行、falseならブロック）
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	// 2. BlackboardのStateが書き換わった瞬間に呼ばれる通知関数
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;

	// エディタ上で指定する比較対象のState（デフォルトでStrafeに設定）
	UPROPERTY(EditAnywhere, Category = "Condition")
	EEnemyState RequiredState = EEnemyState::Strafe;
};
