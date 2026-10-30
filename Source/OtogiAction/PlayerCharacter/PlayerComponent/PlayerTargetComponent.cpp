// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTargetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
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
	UE_LOG(LogTemp, Warning, TEXT("TagetSearchStart"));

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
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("===== TARGET LOCK =====")
		);

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Target Actor : %s"),
			*CurrentTargetActor->GetName()
		);

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Target Location : %s"),
			*CurrentTargetActor->GetActorLocation().ToString()
		);
	}
	else
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("===== TARGET NOT FOUND =====")
		);
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
	if (!Owner)
	{
		return nullptr;
	}

	FVector SearchOrigin = Owner->GetActorLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);

	TArray<FHitResult> HitResults;

	// 開始と終了を同じ位置に設定して周囲の敵をスキャン
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

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();

			// Enemyタグを持っていないActorは除外
			if (!IsValidTargetActor(HitActor))
			{
				continue;
			}

			// Pawnクラス以外を除外
			APawn* TargetPawn = Cast<APawn>(HitActor);
			if (!TargetPawn)
			{
				continue;
			}

			float Dist = FVector::Dist(
				SearchOrigin,
				HitActor->GetActorLocation()
			);

			if (Dist < ClosestDistance)
			{
				ClosestDistance = Dist;
				ClosestEnemy = HitActor;
			}
		}
	}

	return ClosestEnemy;
}

bool UPlayerTargetComponent::IsValidTargetActor(AActor* Actor) const
{
	if (!Actor)return false;

	return Actor->ActorHasTag(TEXT("Enemy"));
}

//最もターゲットしやすい敵を探索する
AActor* UPlayerTargetComponent::FindBestTargetForward()
{
	AActor* Owner = GetOwner();

	if (!Owner)
	{
		return nullptr;
	}

	// プレイヤーコントローラー取得
	APlayerController* PlayerCon =Cast<APlayerController>(Owner->GetNetOwningPlayer()->GetPlayerController(GetWorld()));

	if (!PlayerCon)
	{
		return nullptr;
	}

	// カメラの位置と回転を取得
	FVector CameraLocation;
	FRotator CameraRotation;

	PlayerCon->GetPlayerViewPoint(CameraLocation,CameraRotation);

	// カメラ正面方向
	FVector CameraForward = CameraRotation.Vector();

	// Enemyタグを持つActorをすべて取得
	TArray<AActor*> EnemyActors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(),TEXT("Enemy"),EnemyActors);

	AActor* BestTarget = nullptr;

	// Dot積が大きいほどカメラ正面に近い
	float BestDot = -1.0f;

	for (AActor* Enemy : EnemyActors)
	{
		if (!Enemy)
		{
			continue;
		}

		// プレイヤー自身は除外
		if (Enemy == Owner)
		{
			continue;
		}

		// カメラから敵への方向
		FVector DirectionToEnemy =Enemy->GetActorLocation() - CameraLocation;

		float Distance = DirectionToEnemy.Size();

		// ターゲット範囲外なら除外
		if (Distance > TargetRange)
		{
			continue;
		}

		// 正規化
		DirectionToEnemy.Normalize();

		// カメラ正面と敵方向のDot積
		float Dot =FVector::DotProduct(CameraForward,DirectionToEnemy);

		// カメラの後ろにいる敵は除外
		if (Dot <= 0.0f)
		{
			continue;
		}

		// よりカメラ正面に近い敵を採用
		if (Dot > BestDot)
		{
			BestDot = Dot;
			BestTarget = Enemy;
		}
	}

	return BestTarget;
}

