// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "OtogiAction/Enemy/BossEnemyCharacter.h"
#include "AIController.h"

UAttackDecorator::UAttackDecorator()
{
	NodeName = TEXT("Is Player In AttackRange Decorator");

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
}

bool UAttackDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	//敵の本体を取得
	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter) return false;

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	//BBKeys::PlayerActorからObjectを取得してキャスト
	ACharacter* PlayerCharacter = Cast<ACharacter>(BlackboardComp->GetValueAsObject(FName("PlayerActor")));
	if (!PlayerCharacter) return false;

	//PlayerActorキーからオブジェクトを取得
	UObject* TargetObject = BlackboardComp->GetValueAsObject(FName("PlayerActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());

	bool bInAttackRange = (Distance <= EnemyCharacter->AttackRange);

	return bInAttackRange;

}
