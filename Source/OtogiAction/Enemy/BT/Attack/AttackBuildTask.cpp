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

//タスク実行時に呼ばれる関数
EBTNodeResult::Type UAttackBuildTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* EnemyController = OwnerComp.GetAIOwner();
	ACharacter* EnemyCharacter = EnemyController ? Cast<ACharacter>(EnemyController->GetPawn()) : nullptr;
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	UEnemyAttackBaseComponent* AttackComp = EnemyCharacter->FindComponentByClass<UEnemyAttackBaseComponent>();
	if (!AttackComp || AttackComp->GetMontageCount() == 0) return EBTNodeResult::Failed;

	//Stateをセット
	AttackComp->SetEnemyState();

	// コンボの開始（最初の要素 0 番目を再生）
	CurrentComboIndex = 0;
	bool bStarted = AttackComp->PlayAttackMontageAtIndex(CurrentComboIndex);

	if (bStarted)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
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

	UEnemyAttackBaseComponent* AttackComp = EnemyCharacter->FindComponentByClass<UEnemyAttackBaseComponent>();
	UAnimInstance* AnimInstance = EnemyCharacter->GetMesh() ? EnemyCharacter->GetMesh()->GetAnimInstance() : nullptr;

	if (!AttackComp || !AnimInstance)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//現在のモンタージュ再生が終了したか確認
	if (!AnimInstance->IsAnyMontagePlaying())
	{
		//次のコンボ段数へ進める
		CurrentComboIndex++;

		//まだ再生すべきモンタージュが残っている場合
		if (CurrentComboIndex < AttackComp->GetMontageCount())
		{
			bool bPlayedNext = AttackComp->PlayAttackMontageAtIndex(CurrentComboIndex);
			if (!bPlayedNext)
			{
				//再生失敗時はコンボ中断
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
		else
		{
			//全てのコンボ（配列の全要素）の再生完了！
			//State を Idle に戻して Task を終了
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
}

