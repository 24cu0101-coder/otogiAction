// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChaseTask.generated.h"

class UBossChaseComponent;

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UChaseTask : public UBTTaskNode
{
	GENERATED_BODY()

	UChaseTask();
	
protected:
	// タスク開始時に呼ばれる
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Stateが変わってDecoratorに中断された時に呼ばれる
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// ボスキャラからChaseComponentを取得するヘルパー関数
	class UBossChaseComponent* GetChaseComponent(UBehaviorTreeComponent& OwnerComp) const;
};
