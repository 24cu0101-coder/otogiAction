// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemyStateSubsystem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    Idle,
    AttackBuild,
    AttackPress,
    MoveBuild,
    MovePress,
    MoveRecover
};

UCLASS()
class OTOGIACTION_API UEnemyStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

    // 現在の状態（Blueprintからも参照可能に）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game State")
    EEnemyState CurrentState = EEnemyState::Idle;
};
