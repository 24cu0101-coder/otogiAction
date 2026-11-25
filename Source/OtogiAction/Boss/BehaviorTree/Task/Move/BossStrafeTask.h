// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BossStrafeTask.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UBossStrafeTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBossStrafeTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 見合い行動を続ける時間（秒）
	UPROPERTY(EditAnywhere, Category = "Strafe")
	float StrafeDuration = 2.5f;
private:
	class UBossStrafeComponent* GetStrafeComponent(UBehaviorTreeComponent& OwnerComp) const;

	// 経過時間カウント用
	float ElapsedTime = 0.0f;
};
