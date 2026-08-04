#include "MinionsHitReactionTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"

#include "OtogiAction/minions/MinionsCharacter.h"
#include "OtogiAction/minions/MinionsHitReactionComponent.h"

UMinionsHitReactionTask::UMinionsHitReactionTask()
{
	NodeName = TEXT("Play Minions Hit Reaction");
}

EBTNodeResult::Type UMinionsHitReactionTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("===== BT HIT REACTION TASK ====="));

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AMinionsCharacter* Minions =Cast<AMinionsCharacter>(AIController->GetPawn());

	if (!Minions)
	{
		return EBTNodeResult::Failed;
	}

	BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
	{
		return EBTNodeResult::Failed;
	}

	UMinionsHitReactionComponent* HitComponent =Minions->FindComponentByClass<UMinionsHitReactionComponent>();

	if (!HitComponent)
	{
		return EBTNodeResult::Failed;
	}

	//スタンポイント加算
	HitComponent->AddStunPoint(20.0f);

	// 被弾リアクション再生
	HitComponent->PlayHitReaction(0.0f);

	USkeletalMeshComponent* Mesh = Minions->GetMesh();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;
	UAnimMontage* CurrentMontage =
		AnimInstance ? AnimInstance->GetCurrentActiveMontage() : nullptr;

	// モンタージュが無ければ終了
	if (!AnimInstance || !CurrentMontage)
	{
		Minions->SetIsHitFlg(false);

		BBComp->SetValueAsBool(
			IsHitKey.SelectedKeyName,
			false);

		return EBTNodeResult::Succeeded;
	}

	UBehaviorTreeComponent* BTComp = &OwnerComp;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda(
		[this, BTComp, Minions](UAnimMontage* Montage, bool bInterrupted)
		{
			if (!BTComp)
			{
				return;
			}

			UBlackboardComponent* Blackboard =
				BTComp->GetBlackboardComponent();

			Minions->SetIsHitFlg(false);

			if (Blackboard)
			{
				Blackboard->SetValueAsBool(
					IsHitKey.SelectedKeyName,
					false);
			}

			FinishLatentTask(
				*BTComp,
				EBTNodeResult::Succeeded);
		});

	AnimInstance->Montage_SetEndDelegate(EndDelegate,CurrentMontage);

	return EBTNodeResult::InProgress;
}

void UMinionsHitReactionTask::OnMontageEnded(UAnimMontage* Montage,bool bInterrupted,UBehaviorTreeComponent* OwnerComp)
{
}