// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStrafeTask.h"
#include "AIController.h"
#include "../../../Main/BossCharacter.h"
#include "../../../Component/Move/BossStrafeComponent.h"

UBossStrafeTask::UBossStrafeTask()
{
	NodeName = TEXT("Strafe Task");

	// TickTaskを毎フレーム呼ぶために必須
	bNotifyTick = true;
	// メンバ変数（ElapsedTime）を安全に保持するためにインスタンス化
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBossStrafeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBossStrafeComponent* StrafeComp = GetStrafeComponent(OwnerComp);
	if (!StrafeComp)
	{
		return EBTNodeResult::Failed;
	}

	// タイマーをリセットして見合い処理を有効化
	ElapsedTime = 0.0f;

	// コンポーネントの見合い処理を有効化
	StrafeComp->SetStrafeEnabled(true);

	// StateがStrafeである間は継続実行するため InProgress を返す
	return EBTNodeResult::InProgress;
}

void UBossStrafeTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ElapsedTime += DeltaSeconds;

	// 指定時間が経過したら見合い行動を終了し、次のタスク（Select Next Action）へ進める
	if (ElapsedTime >= StrafeDuration)
	{
		if (UBossStrafeComponent* StrafeComp = GetStrafeComponent(OwnerComp))
		{
			StrafeComp->SetStrafeEnabled(false);
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBossStrafeTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Stateが切り替わって中断されたら、見合い処理を無効化してフォーカス解除
	if (UBossStrafeComponent* StrafeComp = GetStrafeComponent(OwnerComp))
	{
		StrafeComp->SetStrafeEnabled(false);
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

UBossStrafeComponent* UBossStrafeTask::GetStrafeComponent(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return nullptr;

	ABossCharacter* BossChar = Cast<ABossCharacter>(AIController->GetPawn());
	if (!BossChar) return nullptr;

	return BossChar->FindComponentByClass<UBossStrafeComponent>();
}
