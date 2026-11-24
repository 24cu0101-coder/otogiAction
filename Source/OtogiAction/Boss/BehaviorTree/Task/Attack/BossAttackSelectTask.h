// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../../../Status/StateClass.h"

#include "BossAttackSelectTask.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UBossAttackSelectTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBossAttackSelectTask();

protected:
	//タスク実行時に一回だけ呼ばれる処理
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	//ターゲットのBBキー
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	//現在のStateキー
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AIStateKey;

	// 直前と同じStateを連続で選ばないようにするかどうか
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bAvoidSameState = true;

	// 抽選対象のStateをエディタでも設定できるように
	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<EEnemyState> CandidateStates;

};
