// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBuildTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "../../Component/AttackBuildComponent.h"

UAttackBuildTask::UAttackBuildTask() 
{
	NodeName = "AttackBuildTask";

	//TickTaskを有効にするために必要
	bNotifyTick = true;
}

EBTNodeResult::Type UAttackBuildTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* EnemyController = OwnerComp.GetAIOwner();
	ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
	if (!EnemyController || !PlayMontage) return EBTNodeResult::Failed;

	UAttackBuildComponent* EnemyAttackComp = EnemyCharacter->FindComponentByClass<UAttackBuildComponent>();
	if (!EnemyAttackComp) return EBTNodeResult::Failed;

	bool IsAttack = EnemyAttackComp->StartAttackBuild();

	if (IsAttack)
	{
		//アニメーション再生に成功したら攻撃発生を呼ぶ
		EnemyAttackComp->ExecuteAttack();

		//アニメーション再生終了までTickTaskで待機
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Succeeded;
}

void UAttackBuildTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* EnemyController = OwnerComp.GetAIOwner();
	ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
	if (!EnemyCharacter)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UAttackBuildComponent* EnemyAttackComp = EnemyCharacter->FindComponentByClass<UAttackBuildComponent>();

	// Component側で bIsAttacking が false（再生終了）になったらタスク完了を通知
	if (EnemyAttackComp && !EnemyAttackComp->IsAttacking())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

