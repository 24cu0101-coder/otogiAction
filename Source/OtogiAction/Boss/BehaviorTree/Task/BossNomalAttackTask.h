// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BossNomalAttackTask.generated.h"

class UBossBaseAttackComponent;

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UBossNomalAttackTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBossNomalAttackTask();

protected:
	// タスク開始時に呼ばれる
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// タスクが外部から中断（Abort）されたときに呼ばれる
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// 攻撃終了通知を受け取るコールバック
	UFUNCTION()
	void HandleAttackEnded();

	// 実行中のコンポーネントとBTへの弱参照
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	TWeakObjectPtr<UBossBaseAttackComponent> CachedAttackComp;
};
