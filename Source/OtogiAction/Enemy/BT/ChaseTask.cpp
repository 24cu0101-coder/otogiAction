// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "../BossEnemyCharacter.h"
UChaseTask::UChaseTask()
{
    NodeName = TEXT("Chase Player");

    bNotifyTick = true;
}

EBTNodeResult::Type UChaseTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

    if (!AIController || !BBComp)
    {
        return EBTNodeResult::Failed;
    }

    // BlackboardからプレイヤーのActorを取得
    AActor* PlayerActor = Cast<AActor>(BBComp->GetValueAsObject(PlayerActorKey.SelectedKeyName));
    if (!PlayerActor)
    {
        return EBTNodeResult::Failed;
    }

    BBComp->SetValueAsBool(CanChaseKey.SelectedKeyName, true);

    // AIをプレイヤーに向けて移動させる
    AIController->MoveToActor(PlayerActor, 100.0f);

    return EBTNodeResult::InProgress;
}

void UChaseTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 親クラスのTickを呼ぶ
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BBComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Blackboardから常に最新のプレイヤー位置/情報を取得
    AActor* PlayerActor = Cast<AActor>(BBComp->GetValueAsObject(PlayerActorKey.SelectedKeyName));
    APawn* ControlledPawn = AIController->GetPawn();

    if (!PlayerActor || !ControlledPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    //敵の本体を取得
    ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(AIController->GetPawn());
    if (!EnemyCharacter) return;

    //敵の攻撃範囲を追跡範囲の限界とする
    LimitChaseRange = EnemyCharacter->AttackRange;

    // プレイヤーとの距離を計算
    float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerActor->GetActorLocation());

    //攻撃範囲より近づいたら追跡完了とする
    if (Distance <= EnemyCharacter->AttackRange)
    {
        // 移動を停止させて、タスクを「成功」で終了する
        AIController->StopMovement();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        BBComp->SetValueAsBool(CanChaseKey.SelectedKeyName, false);
        return;
    }

    // まだ離れている場合は、毎フレームプレイヤーの位置を追いかけるように再命令
    AIController->MoveToActor(PlayerActor, LimitChaseRange);
}
