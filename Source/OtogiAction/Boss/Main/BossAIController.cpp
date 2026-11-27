// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

//ボスが所有した時
void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		// Blackboardの初期化とBehaviorTreeの実行をまとめて行う関数
		RunBehaviorTree(BehaviorTreeAsset);
	}

	// 即座に取得を試み、失敗したらタイマーで再試行する
	TrySetTargetPlayer();
}

//ボスから離脱した時
void ABossAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

//プレイヤーのPawn情報を取得する関数
void ABossAIController::TrySetTargetPlayer()
{
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (BBComp && PlayerPawn)
	{
		BBComp->SetValueAsObject(TEXT("TargetActor"), PlayerPawn);
		// 取得できたらタイマーを停止
		GetWorldTimerManager().ClearTimer(FindPlayerTimerHandle);
	}
	else if (!FindPlayerTimerHandle.IsValid())
	{
		// まだプレイヤーがいなければ 0.1秒後に再チェック（ループ）
		GetWorldTimerManager().SetTimer(
			FindPlayerTimerHandle,
			this,
			&ABossAIController::TrySetTargetPlayer,
			0.1f,
			true
		);
	}
}
