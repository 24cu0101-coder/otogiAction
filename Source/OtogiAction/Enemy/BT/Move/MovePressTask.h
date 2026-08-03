// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MovePressTask.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UMovePressTask : public UBTTaskNode
{
	GENERATED_BODY()

	UMovePressTask();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	// 再生したいモンタージュ
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> PlayMontage;

};
