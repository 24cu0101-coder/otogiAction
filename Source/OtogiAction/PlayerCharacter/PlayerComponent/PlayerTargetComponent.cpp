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
	AActor* Owner = GetOwner();
	if (!Owner)return nullptr;

	FVector SearchOrigin = Owner->GetActorLocation();

	TArray<AActor*>ActorsToIgnore;
	ActorsToIgnore.Add(Owner);

	TArray <FHitResult> HitResults;

	//開始と終了を同じ位置に設定して周囲の敵をスキャン
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		SearchOrigin,
		SearchOrigin,
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn), 
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
	AActor* ClosestEnemy = nullptr;
	float ClosestDistance = UE_BIG_NUMBER;
	//スキャンに引っかかったEnemyをターゲットに暫定
	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != Owner)
			{
				//Pawnクラス以外をはじく
				APawn* TargetPawn = Cast<APawn>(HitActor);
				if (!TargetPawn) continue;

				float Dist = FVector::Dist(SearchOrigin, HitActor->GetActorLocation());
				if (Dist < ClosestDistance)
				{
					ClosestDistance = Dist;
					ClosestEnemy = HitActor;
				}
			}
		}
	}

	return ClosestEnemy;
}

//最もターゲットしやすい敵を探索する
AActor* UPlayerTargetComponent::FindBestTargetForward()
{
	AActor* Owner = GetOwner();
	if (!Owner) return nullptr;

	//カメラ正面の敵を索敵
	APlayerController* PlayerCon = Cast<APlayerController>(Owner->GetNetOwningPlayer()->GetPlayerController(GetWorld()));
	if (!PlayerCon) return nullptr;

	//カメラの位置と角度
	FVector CameraLocation;
	FRotator CameraRotation;

	//カメラの位置と角度を取得
	PlayerCon->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// カメラの正面ベクトルを使って、スキャンの開始位置と終了位置を計算
	FVector StartLoc = CameraLocation;
	FVector EndLoc = StartLoc + (CameraRotation.Vector() * TargetRange);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);

	TArray<FHitResult> HitResults;

	//敵をスキャン
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		StartLoc,
		EndLoc,
		TargetRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, 
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f
	);

	AActor* BestTarget = nullptr;
	float ClosestDistance = UE_BIG_NUMBER;

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != Owner)
			{
				// 当たったアクターがPawnクラスかチェックする
				APawn* TargetPawn = Cast<APawn>(HitActor);
				if (!TargetPawn) continue;

				float Dist = FVector::Dist(StartLoc, HitActor->GetActorLocation());
				if (Dist < ClosestDistance)
				{
					ClosestDistance = Dist;
					BestTarget = HitActor;
				}
			}
		}
	}
	return BestTarget;
}

