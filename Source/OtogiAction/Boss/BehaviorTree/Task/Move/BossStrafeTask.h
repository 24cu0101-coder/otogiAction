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
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	class UBossStrafeComponent* GetStrafeComponent(UBehaviorTreeComponent& OwnerComp) const;
};
