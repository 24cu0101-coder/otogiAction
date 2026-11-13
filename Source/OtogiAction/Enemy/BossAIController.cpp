// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"

//ボスが所有した時
void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		// Blackboardの初期化とBehaviorTreeの実行をまとめて行う関数
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

//ボスから離脱した時
void ABossAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
