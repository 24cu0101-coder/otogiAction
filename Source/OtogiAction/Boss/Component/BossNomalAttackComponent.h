// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossBaseAttackComponent.h"
#include "BossNomalAttackComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UBossNomalAttackComponent : public UBossBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	virtual void StartAttack_Implementation() override;
	virtual void EndAttack_Implementation() override;
};
