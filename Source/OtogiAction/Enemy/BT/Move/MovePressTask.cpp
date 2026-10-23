// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePressTask.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "../../Component/MovePressComponent.h"

UMovePressTask::UMovePressTask() 
{
	NodeName = "MovePress Task";
	bNotifyTick = true;
}

//MovePressCompを取得して、Comp内のflagを取得してTaskのtrueかfalseかを決める
EBTNodeResult::Type UMovePressTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* EnemyController = OwnerComp.GetAIOwner();
    ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
    if (!EnemyCharacter) return EBTNodeResult::Failed;

    UMovePressComponent* PressComp = EnemyCharacter->FindComponentByClass<UMovePressComponent>();
    if (!PressComp) return EBTNodeResult::Failed;

    //Stateをセット
    PressComp->SetEnemyState();

    bool bStarted = PressComp->StartDashTracking();
    if (bStarted)
    {
        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

void UMovePressTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* EnemyController = OwnerComp.GetAIOwner();
    ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
    if (!EnemyCharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UMovePressComponent* PressComp = EnemyCharacter->FindComponentByClass<UMovePressComponent>();

    if (PressComp && !PressComp->IsDashTracking())
    {
        //完了時に State を Idle に戻す
        if (UWorld* World = OwnerComp.GetWorld())
        {
            if (UGameInstance* GI = World->GetGameInstance())
            {
                if (UEnemyStateSubsystem* StateSubsystem = GI->GetSubsystem<UEnemyStateSubsystem>())
                {
                    StateSubsystem->CurrentState = EEnemyState::Idle;
                }
            }
        }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
