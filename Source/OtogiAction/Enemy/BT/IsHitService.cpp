// Fill out your copyright notice in the Description page of Project Settings.


#include "IsHitService.h"
#include "AIController.h"
#include "../BossEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UIsHitService::UIsHitService()
{
	NodeName = "Check Player In ChaseRange";

	// Tick間隔のデフォルト値
	Interval = 0.2f;
	RandomDeviation = 0.0f;

}

void UIsHitService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return;

	//敵キャラクターを取得
	AAIController* EnemyController = OwnerComp.GetAIOwner();
	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(EnemyController->GetPawn());
	if (!EnemyCharacter) return;

	bool IsHit = EnemyCharacter->GetIsHitFlg();
	BBComp->SetValueAsBool(IsHitKey.SelectedKeyName, IsHit);
}
