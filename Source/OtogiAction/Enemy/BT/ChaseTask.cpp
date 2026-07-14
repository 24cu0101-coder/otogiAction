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
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if (!AIController || !BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    // BlackboardからプレイヤーのActorを取得
    AActor* PlayerActor = Cast<AActor>(BlackboardComp->GetValueAsObject(PlayerActorKey.SelectedKeyName));
    if (!PlayerActor)
    {
        return EBTNodeResult::Failed;
    }

    // AIをプレイヤーに向けて移動させる
    AIController->MoveToActor(PlayerActor, 100.0f);

    // 今回は「移動命令を出した」時点で一旦タスク成功（Succeeded）として返します
    return EBTNodeResult::InProgress;
}

void UChaseTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // 親クラスのTickを呼ぶ
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!AIController || !BlackboardComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Blackboardから常に最新のプレイヤー位置/情報を取得
    AActor* PlayerActor = Cast<AActor>(BlackboardComp->GetValueAsObject(PlayerActorKey.SelectedKeyName));
    APawn* ControlledPawn = AIController->GetPawn();

    if (!PlayerActor || !ControlledPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    //敵の本体を取得
    ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(AIController->GetPawn());
    if (!EnemyCharacter) return;

    // プレイヤーとの距離を計算
    float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerActor->GetActorLocation());

    //攻撃範囲より近づいたら追跡完了とする
    if (Distance <= EnemyCharacter->AttackRange)
    {
        // 移動を停止させて、タスクを「成功」で終了する
        AIController->StopMovement();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // まだ離れている場合は、毎フレームプレイヤーの位置を追いかけるように再命令
    AIController->MoveToActor(PlayerActor, LimitChaseRange);
}
