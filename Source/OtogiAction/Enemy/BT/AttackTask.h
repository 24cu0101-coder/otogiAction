// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackTask.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UAttackTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UAttackTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//アニメーション終了にタイマーから呼び出される関数
	void OnAttackAnimationFinished(TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompPtr);

};
