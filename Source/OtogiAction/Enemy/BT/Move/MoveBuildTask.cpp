// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBuildTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

UMoveBuildTask::UMoveBuildTask()
{
}

EBTNodeResult::Type UMoveBuildTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UMoveBuildTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
