// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackPressTask.generated.h"

class UBlackboardComponent;
class UEnemyBaseComponent;

/**
 * 
 */

UCLASS()
class OTOGIACTION_API UAttackPressTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UAttackPressTask();

	//タスク実行時に流れる関数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 再生したいモンタージュ
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> PlayMontage;

};
