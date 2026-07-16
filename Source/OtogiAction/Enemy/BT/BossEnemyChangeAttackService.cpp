// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyChangeAttackService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "../BossEnemyCharacter.h"

UBossEnemyChangeAttackService::UBossEnemyChangeAttackService()
{
	NodeName = "Check Player In ChaseRange";

	// Tick間隔のデフォルト値
	Interval = 0.2f;
	RandomDeviation = 0.0f;
}

void UBossEnemyChangeAttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return;

	//追跡シーケンスに入っている＝追跡中なので、無条件でタイマーをセット/更新する
	float CurrentReadyTime = BBComp->GetValueAsFloat(AttackReadyTimeKey.SelectedKeyName);

	// まだタイマーがセットされていなければセット
	if (CurrentReadyTime <= 0.0f)
	{
		float FutureReadyTime = GetWorld()->GetTimeSeconds() + RequiredTime;
		BBComp->SetValueAsFloat(AttackReadyTimeKey.SelectedKeyName, FutureReadyTime);
	}
}
