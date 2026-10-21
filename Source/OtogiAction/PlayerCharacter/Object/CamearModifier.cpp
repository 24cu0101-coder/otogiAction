// Fill out your copyright notice in the Description page of Project Settings.


#include "CamearModifier.h"

void UPlayerCamearModifier::UpdateModifier(float DeltaTime, FCameraModifierData& ModifierData)
{
	ElapsedTime += DeltaTime;
}

bool UPlayerCamearModifier::IsFinish() const
{
    if (Duration <= 0.0f)
    {
        return false;
    }

    return ElapsedTime >= Duration;
}
