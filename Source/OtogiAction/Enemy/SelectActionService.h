// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "StateClass.h"
#include "SelectActionService.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API USelectActionService : public UBTService
{
	GENERATED_BODY()
	
public:
	USelectActionService();

protected:
	// サービスの更新間隔ごとに呼ばれる処理
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	//ターゲット（プレイヤー）のBBキー
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	//現在のStateキー
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AIStateKey;

	//UPROPERTY(EditAnywhere, Category = "AI")
	//float AttackRange = 200.0f;

	//UPROPERTY(EditAnywhere, Category = "AI")
	//float StrafeRange = 600.0f;

	// 直前と同じStateを連続で選ばないようにするかどうか
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bAvoidSameState = true;
};
