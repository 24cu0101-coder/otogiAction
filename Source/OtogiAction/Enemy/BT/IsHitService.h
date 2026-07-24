// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "IsHitService.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UIsHitService : public UBTService
{
	GENERATED_BODY()
	
public:
	UIsHitService();

protected:

	//設定したTick間隔で実行される処理
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//プレイヤーのアクターキー
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PlayerActorKey;

	// 攻撃可能になる時刻を保存するキー
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsHitKey;

};
