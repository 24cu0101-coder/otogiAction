// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ChangeActionService.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UChangeActionService : public UBTService
{
	GENERATED_BODY()
	
public:
	UChangeActionService();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// サービスがアクティブ化された時
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// サービスが非アクティブ化された時
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	//PlayerCharacterを参照するためのキー
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "BBKey")
	FBlackboardKeySelector TargetActorKey;
};
