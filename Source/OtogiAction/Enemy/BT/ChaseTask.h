// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChaseTask.generated.h"

class ABossEnemyCharacter;

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

	// Blackboardのキーを指定するためのプロパティ
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CanChaseKey;

	// 最大何秒間追跡するか（エディタから変更可能）
	UPROPERTY(EditAnywhere, Category = "ChaseSettings")
	float MaxChaseTime = 5.0f;

private:
	float ChaseTimer = 0.0f;   // 経過時間カウント用
};
