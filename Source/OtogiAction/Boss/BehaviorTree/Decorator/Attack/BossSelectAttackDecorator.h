// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "../../../Status/StateClass.h"
#include "BossSelectAttackDecorator.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UBossSelectAttackDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBossSelectAttackDecorator();

protected:
	// 条件判定
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	// Blackboardの値が変化した際の通知
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;

	// エディタ上のノードに「AIState == NormalAttack」のように現在の設定を表示する
	virtual FString GetStaticDescription() const override;

	// エディタ上で指定する比較対象のState
	UPROPERTY(EditAnywhere, Category = "Condition")
	EEnemyState RequiredState = EEnemyState::None;
};
