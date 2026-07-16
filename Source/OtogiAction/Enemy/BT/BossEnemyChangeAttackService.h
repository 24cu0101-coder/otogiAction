// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BossEnemyChangeAttackService.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UBossEnemyChangeAttackService : public UBTService
{
	GENERATED_BODY()

public:
    UBossEnemyChangeAttackService();

protected:

    //設定したTick間隔で実行される処理
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PlayerActorKey; //プレイヤーのアクターキー

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AttackReadyTimeKey; // 攻撃可能になる時刻を保存するキー
    
    UPROPERTY(EditAnywhere, Category = "Targeting")
    float RequiredTime = 3.0f; // 範囲内に居続ける必要がある時間
};
