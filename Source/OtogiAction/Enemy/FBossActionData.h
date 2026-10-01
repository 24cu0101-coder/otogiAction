// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FBossActionData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBossActionData : public FTableRowBase
{
    GENERATED_BODY()

    // 技の基本ダメージ量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Parameters")
    float Damage = 0.f;

    // 行動移行・発動条件の最小値（距離やHP割合など）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Parameters")
    float MinTriggerValue = 0.f;

    // 行動移行・発動条件の最大値
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Parameters")
    float MaxTriggerValue = 0.f;

    // 行動の優先度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Parameters")
    float Priority = 0.f;

    // 優先度のスコアが下がり始める距離
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Parameters")
    float FeedOutRange = 0.f;
};