// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAIComponent.h"
#include "AIController.h"
#include "TimerManager.h"
#include "EnemyAttackBaseComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values for this component's properties
UUtilityAIComponent::UUtilityAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Tick関数はOFFにしてタイマーで代用する
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UUtilityAIComponent::BeginPlay()
{
	Super::BeginPlay();

	//タイマーをセット
	OwnerAIController = Cast<AAIController>(GetOwner());
	if (OwnerAIController)
	{
		//指定間隔ごとに評価関数を呼び出す
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandleEvaluate,
			this,
			&UUtilityAIComponent::EvaluateAndExecute,
			EvaluaionInterval,
			true
		);
	}
}


// Called every frame
void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//行動を評価する関数
void UUtilityAIComponent::EvaluateAndExecute()
{
	if (!OwnerAIController) return;

	UBlackboardComponent* BlackboardComp = OwnerAIController->GetBlackboardComponent();
	APawn* OwnerPawn = OwnerAIController->GetPawn();
	if (!OwnerPawn || !BlackboardComp) return;

	UEnemyAttackBaseComponent* BestActions = nullptr;
	float HighestScore = -1.0f;

	//全行動を計算し、最高の物を決定
	for (UEnemyAttackBaseComponent* Action : Actions)
	{
		if (Action)
		{
			//計算したスコアを取得
			float Score = Action->CalculateScore(OwnerAIController, OwnerPawn);

			if (Score > HighestScore)
			{
				//計算したスコアをセット
				//エディターでセットした行動をセット
				HighestScore = Score;
				BestActions = Action;
			}
		}
	}

	//スコアが最も高い行動を選出して実行
	if (BestActions && HighestScore > 0.0f)
	{
		BestActions->SetEnemyState();
	}
}

