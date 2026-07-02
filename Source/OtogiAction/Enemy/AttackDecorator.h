// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AttackDecorator.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UAttackDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UAttackDecorator();

protected:
	//条件のTrue,Falseを判定する関数
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	//エディタ側でBlackboardのCanAttackキーを割り当てるための変数
	UPROPERTY(EditAnywhere, Category = "AI|Blackboard")
	FBlackboardKeySelector CanAttackKey;


};
