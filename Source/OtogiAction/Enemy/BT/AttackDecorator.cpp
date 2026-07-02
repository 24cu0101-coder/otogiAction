// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UAttackDecorator::UAttackDecorator()
{
	NodeName = TEXT("Is Player In AttackRange Decorator");

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
}

bool UAttackDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	//if (!BlackboardComp) return false;

	////PlayerActorキーからオブジェクトを取得
	//UObject* TargetObject = BlackboardComp->GetValueAsObject(PlayerActor.SelectedKeyName);
	//AActor* TargetActor = Cast<AActor>(TargetObject);

	//return (TargetActor != nullptr);

	return false;
}
