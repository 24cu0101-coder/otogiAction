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

public:
	UChaseTask();
	
protected:
	// タスク開始時に呼ばれる
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// タスク実行中に毎フレーム呼ばれる
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Stateが変わってDecoratorに中断された時に呼ばれる
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Behavior Treeエディタ側から追跡完了の距離を調整できるようにする
	UPROPERTY(EditAnywhere, Category = "Chase")
	float CompleteDistance = 150.0f;
private:
	// ボスキャラからChaseComponentを取得するヘルパー関数
	class UBossChaseComponent* GetChaseComponent(UBehaviorTreeComponent& OwnerComp) const;
};
