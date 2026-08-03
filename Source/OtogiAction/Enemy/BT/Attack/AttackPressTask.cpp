// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackPressTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "../../Component/AttackPressComponent.h"

UAttackPressTask::UAttackPressTask()
{
	NodeName = "AttackPressTask";

	//TickTaskを有効にするために必要
	bNotifyTick = true;
}

EBTNodeResult::Type UAttackPressTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* EnemyController = OwnerComp.GetAIOwner();
	ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
	if (!EnemyController || !PlayMontage) return EBTNodeResult::Failed;

	UAttackPressComponent* EnemyAttackComp = EnemyCharacter->FindComponentByClass<UAttackPressComponent>();
	if (!EnemyAttackComp) return EBTNodeResult::Failed;

	//AttackBuildComponentで攻撃処理を呼ぶ
	EnemyAttackComp->ExecuteAttack();
	bool IsAttack = EnemyAttackComp->StartAttackBuild();

	//攻撃アニメーションが終わるまで待機
	if (IsAttack)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Succeeded;
}

void UAttackPressTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* EnemyController = OwnerComp.GetAIOwner();
	ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
	if (!EnemyCharacter)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UAttackPressComponent* EnemyAttackComp = EnemyCharacter->FindComponentByClass<UAttackPressComponent>();

	// Component側で bIsAttacking が false（再生終了）になったらタスク完了を通知
	if (EnemyAttackComp && !EnemyAttackComp->IsAttacking())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
