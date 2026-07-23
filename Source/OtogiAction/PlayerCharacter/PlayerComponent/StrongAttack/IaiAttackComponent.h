// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/BaseAttackComponent.h"
#include "IaiAttackComponent.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UIaiAttackComponent : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:

	//
	void ExecuteIaiAttackAbility();

};
