// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../../../Status/StateClass.h"

#include "BossAttackSelectTask.generated.h"

/**
 * 
 */

// 距離と重みを含めた抽選候補のデータ構造
USTRUCT(BlueprintType)
struct FBossAttackCandidate
{
	GENERATED_BODY()

	// 選出する行動（State）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EEnemyState State = EEnemyState::NormalAttack;

	// 選出の重み（大きいほど選ばれやすい）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0"))
	float Weight = 1.0f;

	// この攻撃が有効になる最小距離（cm）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0"))
	float MinDistance = 0.0f;

	// この攻撃が有効になる最大距離（cm）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0"))
	float MaxDistance = 1000.0f;
};

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
	
	//重み付きの抽選リスト
	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<FBossAttackCandidate> CandidateList;
	
	// すべての候補が距離外だった場合のフォールバック（例：移動や接近など）
	UPROPERTY(EditAnywhere, Category = "AI")
	EEnemyState FallbackState = EEnemyState::None;
};
