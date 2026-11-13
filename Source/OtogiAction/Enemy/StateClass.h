// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle    UMETA(DisplayName = "Idle"),
	Chase   UMETA(DisplayName = "Chase"),
	//Attack  UMETA(DisplayName = "Attack"),
	Strafe	UMETA(DisplayName = "Strafe")
};

class OTOGIACTION_API StateClass
{
public:
	StateClass();
	~StateClass();
};
