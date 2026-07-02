// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "OtogiAction/Enemy/BossEnemyCharacter.h"
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
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	//BBKeys::PlayerActorからObjectを取得してキャスト
	ACharacter* PlayerCharacter = Cast<ACharacter>(BlackboardComp->GetValueAsObject(FName("PlayerActor")));
	if (!PlayerCharacter) return false;

	float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());

	bool bInSightRange = (Distance <= EnemyCharacter->SightRange);

	//指定されたkeySelectorから現在のBool値を取得
	BlackboardComp->SetValueAsBool(FName("CanChase"), bInSightRange);

	//Trueならこの先のシーケンスを実行、Falseなら実行しない
	return bInSightRange;
}
