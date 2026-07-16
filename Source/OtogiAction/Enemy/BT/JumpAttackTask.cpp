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

		//プレイヤーとの距離を算出
		float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
		//Montageの再生時間を取得
		float AnimationTime = EnemyCharacter->GetPlayJumpAttackMontageTime();
		//敵の移動スピードを元に戻すため、MaxWalkSpeedの初期値を取得
		DefaultSpeed = EnemyCharacter->GetMovementSpeed();
		//Montage再生中のスピードをセット
		EnemyCharacter->SetMovementSpeed(Distance / AnimationTime);

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

	//敵の移動スピードをもとに戻す
	EnemyCharacter->SetMovementSpeed(DefaultSpeed);

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
	}
}
