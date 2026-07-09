// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChaseTask.generated.h"

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
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//アニメーション終了にタイマーから呼び出される関数
	void OnAttackAnimationFinished(TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompPtr);

	//タスク実行中、毎フレーム呼ばれる
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Blackboardのキーを指定するためのプロパティ
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PlayerActorKey;

	// 追跡を完了（成功）とする接近距離
	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptanceRadius = 100.0f;
};
