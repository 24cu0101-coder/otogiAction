// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBuildTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

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

	UAnimInstance* AnimInstance = EnemyCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;

	// NodeMemory に再生状態を保存
	FBTPlayAnimationTaskMemory* MyMemory = CastInstanceNodeMemory<FBTPlayAnimationTaskMemory>(NodeMemory);
	MyMemory->AnimInstance = AnimInstance;
	MyMemory->PlayingMontage = PlayMontage;

	// モンタージュ再生を開始
	float Duration = AnimInstance->Montage_Play(PlayMontage);
	if (Duration <= 0.0f)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

void UAttackBuildTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTPlayAnimationTaskMemory* MyMemory = CastInstanceNodeMemory<FBTPlayAnimationTaskMemory>(NodeMemory);

	if (!MyMemory->AnimInstance.IsValid() || !MyMemory->PlayingMontage.IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 対象のモンタージュがまだ再生中かチェック
	const bool bIsPlaying = MyMemory->AnimInstance->Montage_IsPlaying(MyMemory->PlayingMontage.Get());

	if (!bIsPlaying)
	{
		// 再生が完了（または中断）したらタスク成功として終了をエンジンに通知
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
