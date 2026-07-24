// Fill out your copyright notice in the Description page of Project Settings.


#include "HitReactionDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../BossEnemyCharacter.h"

UHitReactionDecorator::UHitReactionDecorator()
{
	NodeName = TEXT("Hit Reaction Decorator");

}

bool UHitReactionDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	//敵の本体を取得
	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter) return false;

	const UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return false;

	bool IsHitNow = BBComp->GetValueAsBool(IsHitKey.SelectedKeyName);

	return IsHitNow;
}
