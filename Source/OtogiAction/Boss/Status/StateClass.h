// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Strafe			UMETA(DisplayName = "Strafe"),
	NormalAttack  UMETA(DisplayName = "NormalAttack"),
	ComboAttack UMETA(DisplayName = "ComboAttack"),
	BigAttack UMETA(DisplayName = "BigAttack"),
	LongRangeAttack UMETA(DisplayName = "LongRangeAttack"),
	DelayAttack UMETA(DisplayName = "DelayAttack"),
	None UMETA(DisplayName = "None")
};

class OTOGIACTION_API StateClass
{
public:
	StateClass();
	~StateClass();
};
