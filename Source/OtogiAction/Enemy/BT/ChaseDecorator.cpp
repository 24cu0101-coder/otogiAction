// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../BossEnemyCharacter.h"
#include "AIController.h"

UChaseDecorator::UChaseDecorator()
{
	NodeName = TEXT("Is Player In SightRange Decorator");

	//デコレーターがBlackboardの値を監視して自動で中断（Abort）を実行できるようにする設定
	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;

}

bool UChaseDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	//敵の本体を取得
	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter) return false;

	//Blackboardコンポーネントを取得
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return false;

	//BBKeys::PlayerActorからObjectを取得してキャスト
	ACharacter* PlayerCharacter = Cast<ACharacter>(BBComp->GetValueAsObject(FName("PlayerActor")));
	if (!PlayerCharacter) return false;

	//プレイヤーとの距離を判定
	float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());

	//視界内にプレイヤーがいたらtrue
	bool bInSightRange = (Distance <= EnemyCharacter->SightRange);

	//Trueならこの先のシーケンスを実行、Falseなら実行しない
	return bInSightRange;
}
