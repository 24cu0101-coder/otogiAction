// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeActionService.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../EnemyStateSubsystem.h"
#include "../Component/EnemyAttackBaseComponent.h"

UChangeActionService::UChangeActionService()
{
	NodeName = "ChangeAction Service";

	// TickNode を有効化
	bNotifyTick = true;

	// OnBecomeRelevant / OnCeaseRelevant を有効化する場合に必要
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	// デフォルトの間隔を設定（エディタから調整可能）
	Interval = 0.2f;       // 0.5秒おきに実行
	RandomDeviation = 0.1f; // ±0.1秒の揺らぎ（負荷分散用）
}

void UChangeActionService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//BlackboardへのTargetActor設定処理
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		//プレイヤーの情報をセット
		BlackboardComp->SetValueAsObject(TargetActorKey.SelectedKeyName, PlayerCharacter);
	}
	else
	{
		// 見つからなければクリア
		BlackboardComp->ClearValue(TargetActorKey.SelectedKeyName);
	}

	//UtilityAI:距離スコア計算とCurrentStateの更新処理
	AAIController* AIController = OwnerComp.GetAIOwner();
	ACharacter* EnemyCharacter = AIController ? Cast<ACharacter>(AIController->GetPawn()) : nullptr;
	if (!EnemyCharacter) return;

	UWorld* World = OwnerComp.GetWorld();
	UGameInstance* GI = World ? World->GetGameInstance() : nullptr;
	UEnemyStateSubsystem* StateSubsystem = GI ? GI->GetSubsystem<UEnemyStateSubsystem>() : nullptr;
	if (!StateSubsystem) return;

	//すでに攻撃中・移動中などのアクションを実行中であればスコア計算をスキップして完了をまつ
	if (StateSubsystem->CurrentState != EEnemyState::Idle)
	{
		return;
	}

	//エネミーにアタッチされている全ての行動コンポーネントを取得
	TArray<UEnemyAttackBaseComponent*> ActionComps;
	EnemyCharacter->GetComponents<UEnemyAttackBaseComponent>(ActionComps);

	float BestScore = -1.f;
	EEnemyState BestState = EEnemyState::Idle;

	//各コンポーネントのスコアを計算
	for (UEnemyAttackBaseComponent* Comp : ActionComps)
	{
		if (!Comp) continue;

		//距離スコアを計算
		float Score = Comp->CalculateScore(AIController, EnemyCharacter);

		if (Score > BestScore && Score > 0.0f)
		{
			BestScore = Score;
			//コンポーネントが保持する目標ステートを取得
			BestState = Comp->GetTargetState();
		}
	}

	//最高スコアのアクションが存在する場合、CurrentStateを更新
	if (BestState != EEnemyState::Idle)
	{
		StateSubsystem->CurrentState = BestState;
	}
}

void UChangeActionService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UChangeActionService::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}
