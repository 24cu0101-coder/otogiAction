// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MovePressDecorator.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UMovePressDecorator : public UBTDecorator
{
	GENERATED_BODY()
	

public:
	UMovePressDecorator();

protected:
	//条件のTrue,Falseを判定する関数
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
