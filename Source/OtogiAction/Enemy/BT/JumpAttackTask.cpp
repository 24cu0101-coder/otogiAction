// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpAttackTask.h"
#include "AIController.h"
#include "../EnemyAttackBaseComponent.h"
#include "../BossEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UJumpAttackTask::UJumpAttackTask()
{
	NodeName = TEXT("Jump Attack Task");

	//時間を要するタスクであることを示すフラグを立てる
	bNotifyTick = false;

}

EBTNodeResult::Type UJumpAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* EnemyController = OwnerComp.GetAIOwner();

	//AIControllerからBlackboardコンポーネントを取得
	BBComp = EnemyController->GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;

	if (!AttackClass) return EBTNodeResult::Failed;

	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;
	if (!EnemyPawn) return EBTNodeResult::Failed;

	EnemyCharacter = Cast<ABossEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	//BBKeys::PlayerActorからObjectを取得してキャスト
	ACharacter* PlayerCharacter = Cast<ACharacter>(BBComp->GetValueAsObject(FName("PlayerActor")));
	if (!PlayerCharacter) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;

	//コンポーネントを動的に生成して、Pawnにアタッチする
	UEnemyAttackBaseComponent* NewAttack = NewObject<UEnemyAttackBaseComponent>(EnemyPawn, AttackClass);
	if (NewAttack)
	{
		NewAttack->RegisterComponent();

		//終了イベントをバインド
		NewAttack->OnAttackFinished.AddDynamic(this, &UJumpAttackTask::OnAttackCompleted);

		//プレイヤーを注視
		AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(PlayerActorKey.SelectedKeyName));
		if (TargetActor)
		{
			EnemyController->SetFocus(TargetActor);
		}

		//プレイヤーへ飛びつく処理
		{
			//プレイヤーへの方向ベクトルを計算（水平方向のみ）
			FVector StartLoc = EnemyCharacter->GetActorLocation();
			FVector TargetLoc = PlayerCharacter->GetActorLocation();
			FVector Dir = TargetLoc - StartLoc;
			Dir.Z = 0.0f; // 上下の高低差は無視して水平の向きを出す
			float HorizontalDistance = Dir.Size();
			Dir.Normalize();

			//アニメーションの時間を取得
			float AnimationTime = EnemyCharacter->GetPlayJumpAttackMontageTime();
			if (AnimationTime <= 0.0f) AnimationTime = 1.0f; // 安全対策

			//水平方向と垂直方向（ジャンプの高さ）の速度を計算
			// アニメーションの時間内にプレイヤーの元へ届く水平速度
			float HorizontalVelocity = HorizontalDistance / AnimationTime;
			FVector LaunchVelocity = Dir * HorizontalVelocity;

			//上方向への飛び上がり速度（ここの数値（例: 800.f）でジャンプの高さを調整してください）
			LaunchVelocity.Z = 800.0f;
		}

		//Blackboardの値をセット
		BBComp->SetValueAsBool(CanJumpAttackkey.SelectedKeyName, true);

		//攻撃を実行
		NewAttack->ExecuteAttack();


		//攻撃が終わるまでここで待機させる
		return EBTNodeResult::InProgress;

	}

	return EBTNodeResult::Failed;

}


void UJumpAttackTask::OnAttackCompleted(bool bSuccess)
{
	//BTにタスクが完了したことを通知
	EBTNodeResult::Type Result = bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	FinishLatentTask(*CachedOwnerComp, Result);

	AAIController* AIController = CachedOwnerComp->GetAIOwner();
	if (AIController)
	{
		//注視を解除
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	if (BBComp)
	{
		// BlackboardのTimerEndTime をリセット
		BBComp->SetValueAsFloat(TimerKey.SelectedKeyName, 0.0f);
		BBComp->SetValueAsBool(CanJumpAttackkey.SelectedKeyName, false);
	}
}
