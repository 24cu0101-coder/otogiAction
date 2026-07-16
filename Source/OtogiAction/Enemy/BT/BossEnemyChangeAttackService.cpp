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
	AAIController* EnemyController = OwnerComp.GetAIOwner();
	if (!BBComp || !EnemyController) return;

	APawn* ControlledPawn = EnemyController->GetPawn();
	//BBKeys::PlayerActorからObjectを取得してキャスト
	ACharacter* PlayerCharacter = Cast<ACharacter>(BBComp->GetValueAsObject(FName("PlayerActor")));
	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (ControlledPawn && PlayerCharacter)
	{
		float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerCharacter->GetActorLocation());

		if (Distance <= EnemyCharacter->SightRange)
		{
			//現在の経過時間
			float CurrentReadyTime = BBComp->GetValueAsFloat(AttackReadyTimeKey.SelectedKeyName);

			// まだタイマーがセットされていなければセット
			if (CurrentReadyTime <= 0.0f)
			{
				float FutureReadyTime = GetWorld()->GetTimeSeconds() + RequiredTime;
				BBComp->SetValueAsFloat(AttackReadyTimeKey.SelectedKeyName, FutureReadyTime);
			}
		}
		else
		{
			//範囲外に出てしまった場合タイマー終了時刻をリセット
			BBComp->ClearValue(AttackReadyTimeKey.SelectedKeyName);
		}
	}
	else
	{
		BBComp->ClearValue(AttackReadyTimeKey.SelectedKeyName);
	}
}
