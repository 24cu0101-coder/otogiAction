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

    ChaseTimer = 0.0f;

    BBComp->SetValueAsBool(CanChaseKey.SelectedKeyName, true);

    // AIをプレイヤーに向けて移動させる
    AIController->MoveToActor(PlayerActor, 0.0f);

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

    // 【追加】経過時間を更新
    ChaseTimer += DeltaSeconds;

    // 1. タイムアウト（時間切れ）の判定
    if (ChaseTimer >= MaxChaseTime)
    {
        // 追跡時間を超えたので、移動を止めてタスクを終了
        AIController->StopMovement();

        // ここで「追跡終了」とするためにブラックボードを更新
        BBComp->SetValueAsBool(CanChaseKey.SelectedKeyName, false);

        // タスクを「失敗(Failed)」または「成功(Succeeded)」で抜けます。
        // ※「失敗」で抜けると、ビヘイビアツリーは上位ノード（Selector）に戻り、別の行動を選び直せます。
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // 2. 攻撃範囲に入ったかの判定
    float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerActor->GetActorLocation());
    float EffectiveAttackRange = EnemyCharacter->AttackRange;

    DrawDebugCircle(
        GetWorld(),
        EnemyCharacter->GetActorLocation(),
        EffectiveAttackRange,
        32,
        FColor::Green,
        false,
        0,
        0,
        2.f,
        FVector(1, 0, 0),
        FVector(0, 1, 0),
        false
    );

    //攻撃範囲より近づいたら追跡完了とする
    if (Distance <= EffectiveAttackRange)
    {
        // 移動を停止させて、タスクを「成功」で終了する
        AIController->StopMovement();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        BBComp->SetValueAsBool(CanChaseKey.SelectedKeyName, false);
        return;
    }
}
