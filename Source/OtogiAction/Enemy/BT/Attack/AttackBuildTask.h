// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackBuildTask.generated.h"

class UBlackboardComponent;
class UEnemyBaseComponent;

/**
 * 
 */

UCLASS()
class OTOGIACTION_API UAttackBuildTask : public UBTTaskNode
{
	GENERATED_BODY()
	
	UAttackBuildTask();

public:
	//タスク実行時に流れる関数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	//現在進行中のコンボインデックス
	int32 CurrentComboIndex = 0;
};
