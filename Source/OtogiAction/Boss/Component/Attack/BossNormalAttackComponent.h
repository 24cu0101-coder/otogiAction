// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Component/Base/BossBaseAttackComponent.h"
#include "BossNormalAttackComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UBossNormalAttackComponent : public UBossBaseAttackComponent
{
	GENERATED_BODY()
public:
	virtual void StartAttack_Implementation() override;
	virtual void EndAttack_Implementation() override;

};
