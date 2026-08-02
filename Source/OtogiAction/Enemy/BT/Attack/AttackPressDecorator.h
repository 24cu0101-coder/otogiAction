// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AttackPressDecorator.generated.h"

/**
 * 
 */

class UEnemyStateSubsystem;

UCLASS()
class OTOGIACTION_API UAttackPressDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UAttackPressDecorator();

protected:
	//条件のTrue,Falseを判定する関数
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
