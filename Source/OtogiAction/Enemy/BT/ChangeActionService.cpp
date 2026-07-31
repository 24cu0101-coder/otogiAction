// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeActionService.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UChangeActionService::UChangeActionService()
{
	NodeName = "ChangeAction Service";

	// TickNode を有効化
	bNotifyTick = true;

	// OnBecomeRelevant / OnCeaseRelevant を有効化する場合に必要
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	// デフォルトの間隔を設定（エディタから調整可能）
	Interval = 0.2f;       // 0.5秒おきに実行
	RandomDeviation = 0.1f; // ±0.1秒の揺らぎ（負荷分散用）
}

void UChangeActionService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		//プレイヤーの情報をセット
		BlackboardComp->SetValueAsObject(TargetActorKey.SelectedKeyName, PlayerCharacter);
	}
	else
	{
		// 見つからなければクリア
		BlackboardComp->ClearValue(TargetActorKey.SelectedKeyName);
	}


}

void UChangeActionService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
}

void UChangeActionService::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
}
