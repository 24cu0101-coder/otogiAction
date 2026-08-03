// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MoveBuildDecorator.generated.h"

/**
 * 
 */

class UEnemyStateSubsystem;

UCLASS()
class OTOGIACTION_API UMoveBuildDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UMoveBuildDecorator();

protected:
	//条件のTrue,Falseを判定する関数
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
