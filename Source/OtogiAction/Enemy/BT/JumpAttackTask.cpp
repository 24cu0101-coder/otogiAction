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
	//メンバ変数(CachedOwnerComp)に状態を保存するために必要
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UJumpAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* EnemyController = OwnerComp.GetAIOwner();

	//AIControllerからBlackboardコンポーネントを取得
	UBlackboardComponent* BBComp = EnemyController->GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;

	if (!AttackClass) return EBTNodeResult::Failed;

	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;
	if (!EnemyPawn) return EBTNodeResult::Failed;

	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(EnemyPawn);
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	//BBKeys::PlayerActorからObjectを取得してキャスト
	ACharacter* PlayerCharacter = Cast<ACharacter>(BBComp->GetValueAsObject(FName("PlayerActor")));
	if (!PlayerCharacter) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;

	//デリゲートをバインド
	NotifyHandle = EnemyCharacter->OnJumpAttackNotify.AddUObject(this, &UJumpAttackTask::OnHitNotifyReceived, &OwnerComp, EnemyCharacter);

	//コンポーネントを動的に生成して、Pawnにアタッチする
	UEnemyAttackBaseComponent* NewAttack = EnemyCharacter->FindComponentByClass<UEnemyAttackBaseComponent>();
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

		//Blackboardの値をセット
		BBComp->SetValueAsBool(CanJumpAttackkey.SelectedKeyName, true);

		//攻撃を実行
		NewAttack->ExecuteAttack();


		//攻撃が終わるまでここで待機させる
		return EBTNodeResult::InProgress;

	}

	return EBTNodeResult::Failed;

}

//Notifyを受け取る関数
void UJumpAttackTask::OnHitNotifyReceived(UBehaviorTreeComponent* OwnerComp, ABossEnemyCharacter* EnemyChar)
{
	if (!OwnerComp || !EnemyChar) return;

	AAIController* EnemyController = CachedOwnerComp->GetAIOwner();

	//AIControllerからBlackboardコンポーネントを取得
	UBlackboardComponent* BBComp = EnemyController->GetBlackboardComponent();
	if (!BBComp) return;

	//二重発火やメモリ残存を防ぐため真っ先に解除する
	EnemyChar->OnJumpAttackNotify.Remove(NotifyHandle);

	//BBKeys::PlayerActorからObjectを取得してキャスト
	ACharacter* PlayerCharacter = Cast<ACharacter>(BBComp->GetValueAsObject(FName("PlayerActor")));

	//ヒット判定やダメージ適用処理（例: TraceやDamageの呼び出し）
	JumpOn(PlayerCharacter);

	//Behavior Tree に Task 完了を伝える (Success)
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

//プレイヤーへ飛びつく処理
void UJumpAttackTask::JumpOn(ACharacter* PlayerCharacter)
{
	AAIController* EnemyController = CachedOwnerComp->GetAIOwner();
	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;
	if (!EnemyPawn) return;

	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(EnemyPawn);
	if (!EnemyCharacter) return;

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

	EnemyCharacter->LaunchCharacter(LaunchVelocity, false, false);
}

EBTNodeResult::Type UJumpAttackTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* EnemyController = OwnerComp.GetAIOwner();

	UBlackboardComponent* BBComp = EnemyController->GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;

	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;
	if (!EnemyPawn) return EBTNodeResult::Failed;

	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(EnemyPawn);
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	if (EnemyController)
	{
		//注視を解除
		EnemyController->ClearFocus(EAIFocusPriority::Gameplay);

		if (EnemyCharacter && EnemyCharacter->GetMesh())
		{
			if (UAnimInstance* AnimInstance = EnemyCharacter->GetMesh()->GetAnimInstance())
			{
				// 安全のためデリゲート解除 & アニメーション停止
				EnemyCharacter->OnJumpAttackNotify.Remove(NotifyHandle);

				// 割込みが入ったら現在再生中の攻撃モンタージュを即座に停止する
				AnimInstance->Montage_Stop(0.1f, nullptr); // 0.1秒でブレンドアウト

			}
		}
	}

	//Blackboardの攻撃可能フラグを下げておく
	if (BBComp)
	{
		BBComp->SetValueAsBool(CanJumpAttackkey.SelectedKeyName, false);
	}

	//明示的にBTへ「中断完了」を伝える
	FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);

	return EBTNodeResult::Aborted;
}

void UJumpAttackTask::OnAttackCompleted(bool bSuccess)
{
	AAIController* EnemyController = CachedOwnerComp->GetAIOwner();

	UBlackboardComponent* BBComp = EnemyController->GetBlackboardComponent();
	if (!BBComp) return;

	//BTにタスクが完了したことを通知
	EBTNodeResult::Type Result = bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	FinishLatentTask(*CachedOwnerComp, Result);

	if (EnemyController)
	{
		//注視を解除
		EnemyController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	if (BBComp)
	{
		// BlackboardのTimerEndTime をリセット
		BBComp->SetValueAsFloat(TimerKey.SelectedKeyName, 0.0f);
		BBComp->SetValueAsBool(CanJumpAttackkey.SelectedKeyName, false);
	}
}

