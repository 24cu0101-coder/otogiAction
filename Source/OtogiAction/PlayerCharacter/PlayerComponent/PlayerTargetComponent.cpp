// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTargetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPlayerTargetComponent::UPlayerTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentTargetActor = nullptr;
}


// Called when the game starts
void UPlayerTargetComponent::BeginPlay()
{
	Super::BeginPlay();
}

//ターゲットロックオン開始解除
void UPlayerTargetComponent::ToggleTargetLock()
{
	//すでにターゲットしていたら解除する
	if (IsTargeting())
	{
		CurrentTargetActor = nullptr;
		UE_LOG(LogTemp, Log, TEXT("TargetLocked"));
		return;
	}
	//ターゲットしていないなら範囲内から探す
	CurrentTargetActor = FindBestTargetForward();

	if (CurrentTargetActor)
	{
		UE_LOG(LogTemp, Log, TEXT("TargetLockON : %s"), *CurrentTargetActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("TargetNotfound"));
	}
}

//攻撃時の吸い寄せ
AActor* UPlayerTargetComponent::GetSoftLockTarget(float SoftLockRadius)
{
	//サークル範囲内の敵を索敵
	AActor* CloseEnemy = GetCloseEnemyInRadius(SoftLockRadius);
	if (CloseEnemy)
	{
		return CloseEnemy;
	}
	//サークル内にいなくてターゲット中なら
	if (IsTargeting())
	{
		AActor* HardTarget = GetCurrentTargetActor();
		if (HardTarget)
		{
			float Dist = FVector::Dist(GetOwner()->GetActorLocation(), HardTarget->GetActorLocation());

			//ターゲット中の敵がサークル範囲内にいないなら吸い寄せない
			if (Dist > SoftLockRadius)
			{
				return nullptr;
			}
			return HardTarget;
		}
	}
	return nullptr;
}

AActor* UPlayerTargetComponent::GetCloseEnemyInRadius(float Radius)
{
	return nullptr;
}

AActor* UPlayerTargetComponent::FindBestTargetForward()
{
	return nullptr;
}

