// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"

ABossEnemyAI::ABossEnemyAI()
{
	//AIPerceptionコンポーネントを作成
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	//視覚設定の作成とパラメータ設定
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1200.f;									//視界の最大距離
		SightConfig->LoseSightRadius = 1500.f;							//見失う距離
		SightConfig->PeripheralVisionAngleDegrees = 180.f;				//視野角の半分
		SightConfig->SetMaxAge(5.0f);															//記憶の生存時間

		//敵・味方・中立のどれを検知するか
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//コンポーネントに視覚設定を登録
		AIPerceptionComp->ConfigureSense(*SightConfig);
		AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}

}

void ABossEnemyAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// デリゲートのバインド
	if (AIPerceptionComp)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ABossEnemyAI::OnTargetPerceptionUpdated);
	}

	EnemyCharacter = Cast<ABossEnemyCharacter>(InPawn);
	if (EnemyCharacter && SightConfig && AIPerceptionComp)
	{
		SightConfig->SightRadius = EnemyCharacter->SightRange;
		SightConfig->LoseSightRadius = EnemyCharacter->LoseSightRange;
		SightConfig->PeripheralVisionAngleDegrees = EnemyCharacter->SightDegrees;

		// 設定を再認識させるためにランタイムで更新を要求する
		AIPerceptionComp->ConfigureSense(*SightConfig);
	}
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}

}

void ABossEnemyAI::Tick(float DeltaTime)
{
}

//プレイヤーが視界内に入ったら動く関数
void ABossEnemyAI::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// ターゲットがプレイヤーかどうかの判定（タグやキャストなどでチェック）
	ACharacter* PlayerCharacter = Cast<ACharacter>(Actor);
	if (!PlayerCharacter) return;

	UBlackboardComponent* BBComp = GetBlackboardComponent();
	if (!BBComp) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		BBComp->SetValueAsObject(TargetActorKeyName, Actor);
		// 視界に入った時の処理（例: Blackboardの値を更新、追跡開始など）
		UE_LOG(LogTemp, Log, TEXT("Player Sensed!: %s"), *Actor->GetName());
	}
	else
	{
		BBComp->ClearValue(TargetActorKeyName);
		// 視界から外れた時の処理（例: 追跡中断など）
		UE_LOG(LogTemp, Log, TEXT("Player Lost!: %s"), *Actor->GetName());
	}
}
