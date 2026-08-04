// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveBuildTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "../../Component/MoveBuildComponent.h"

UMoveBuildTask::UMoveBuildTask()
{
	NodeName = "MoveBuild Task";
	bNotifyTick = true;
}

EBTNodeResult::Type UMoveBuildTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* EnemyController = OwnerComp.GetAIOwner();
    ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
    if (!EnemyCharacter) return EBTNodeResult::Failed;

    UMoveBuildComponent* BuildComp = EnemyCharacter->FindComponentByClass<UMoveBuildComponent>();
    if (!BuildComp) return EBTNodeResult::Failed;

    //追跡を開始
    bool bStarted = BuildComp->StartTracking();

    //追跡が開始されていたらInProgressを返す
    if (bStarted)
    {
        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

void UMoveBuildTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* EnemyController = OwnerComp.GetAIOwner();
    ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
    if (!EnemyCharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UMoveBuildComponent* BuildComp = EnemyCharacter->FindComponentByClass<UMoveBuildComponent>();
    if (!BuildComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    //追跡状態の更新処理を呼ぶ
    BuildComp->UpdateTrackingStatus();

    // 追跡完了判定（または目標到達時など）
    if (!BuildComp->IsTracking())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }

}
