// Fill out your copyright notice in the Description page of Project Settings.


#include "HitReactionTask.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"
#include "../BossEnemyHitReactionComponent.h"
#include "../BossEnemyCharacter.h"

UHitReactionTask::UHitReactionTask()
{
	NodeName = TEXT("Play Hit Reaction");
}

EBTNodeResult::Type UHitReactionTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* EnemyController = OwnerComp.GetAIOwner();
	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;
	if (!EnemyPawn) return EBTNodeResult::Failed;

	//AIControllerからBlackboardコンポーネントを取得
	BBComp = EnemyController->GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;

	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(EnemyPawn);
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	UBossEnemyHitReactionComponent* HitComp = EnemyPawn->FindComponentByClass<UBossEnemyHitReactionComponent>();
	if(!HitComp) return EBTNodeResult::Failed;

	//HitComponent の再生処理を呼び出す
	HitComp->PlayHitReaction(0.0f);

	//再生されたモンタージュを取得して終了待機処理を組む
	USkeletalMeshComponent* Mesh = EnemyCharacter->GetMesh();
	UAnimInstance* AnimInstance = Mesh ? Mesh->GetAnimInstance() : nullptr;

	// 現在再生中のモンタージュを取得
	UAnimMontage* CurrentMontage = AnimInstance ? AnimInstance->GetCurrentActiveMontage() : nullptr;

	if (AnimInstance && CurrentMontage)
	{
		UBehaviorTreeComponent* MyOwnerComp = &OwnerComp;

		// アニメーション終了時のDelegateをセット
		FOnMontageEnded EndedDelegate;
		EndedDelegate.BindLambda([this, MyOwnerComp,EnemyCharacter](UAnimMontage* Montage, bool bInterrupted)
			{
				if (MyOwnerComp)
				{	//被弾フラグリセット
					EnemyCharacter->SetIsHitFlg(false);
					BBComp->SetValueAsBool(IsHitKey.SelectedKeyName, false);

					// モンタージュ終了後にタスクを「成功」として完了し、BTを復帰させる
					FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
				}
			});

		AnimInstance->Montage_SetEndDelegate(EndedDelegate, CurrentMontage);

		return EBTNodeResult::InProgress;
	}

	// モンタージュが再生されなかった場合は即時成功として返す
	return EBTNodeResult::Succeeded;


	//if (HitComp)
	//{
	//	HitComp->RegisterComponent();

	//	float num = 0.0;

	//	//HitReactionコンポーネントのモンタージュ再生関数を呼び出す
	//	HitComp->PlayHitReaction(num);

	//	//Hitアニメーションが終わるまでここで待機させる
	//	return EBTNodeResult::InProgress;
	//}

	//BBComp->SetValueAsBool(IsHitKey.SelectedKeyName, false);

	//return EBTNodeResult::Failed;
}

void UHitReactionTask::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
}
