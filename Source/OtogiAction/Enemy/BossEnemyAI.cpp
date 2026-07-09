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

	EnemyCharacter = CreateDefaultSubobject<ABossEnemyCharacter>(TEXT("EnemyCharacterComponent"));

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

void ABossEnemyAI::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	// 検知したアクターがプレイヤーキャラクターか判定
	ACharacter* PlayerCharacter = Cast<ACharacter>(Actor);
	if (!PlayerCharacter || !PlayerCharacter->IsPlayerControlled()) return;

	//親クラスからBlackboardを取得
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return;

	//今操作している敵の本体を取得
	EnemyCharacter = Cast<ABossEnemyCharacter>(GetPawn());


	//視界に入ったかどうか
	if (Stimulus.WasSuccessfullySensed())
	{
		//プレイヤーキャラクターをメンバ変数に保存
		TargetActor = PlayerCharacter;

		//プレイヤーを発見したら、そのアクター注視する
		SetFocus(TargetActor);

		//Blackboardにプレイヤーを登録
		BlackboardComponent->SetValueAsObject(BBKeys::PlayerActor, TargetActor);

	}
	//視界外に出たら
	else
	{
		//注視を解除
		ClearFocus(EAIFocusPriority::Gameplay);

		BlackboardComponent->ClearValue(BBKeys::PlayerActor);
		BlackboardComponent->SetValueAsBool(BBKeys::CanAttack, false);

		TargetActor = nullptr;
	}

}
