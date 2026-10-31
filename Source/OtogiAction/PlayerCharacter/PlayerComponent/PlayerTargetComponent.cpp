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

void UPlayerTargetComponent::ChangeTarget(const FVector2D& InputDirection)
{
	if (!IsTargeting())
	{
		return;
	}

	AActor* NewTarget = FindNextTarget(InputDirection);

	if (!IsValid(NewTarget))
	{
		return;
	}

	// ターゲット変更
	CurrentTargetActor = NewTarget;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("===== TARGET CHANGE =====")
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("New Target : %s"),
		*CurrentTargetActor->GetName()
	);
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
	APlayerController* PlayerController =Cast<APlayerController>(GetOwner()->GetInstigatorController());

	if (!PlayerController)
	{
		return nullptr;
	}

	FVector ViewLocation;
	FRotator ViewRotation;

	PlayerController->GetPlayerViewPoint(ViewLocation,ViewRotation);

	const FVector ViewForward =ViewRotation.Vector().GetSafeNormal();

	TArray<AActor*> EnemyActors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName("Enemy"),EnemyActors);

	AActor* BestTarget = nullptr;
	float BestDot = TargetDotThreshold;

	for (AActor* Enemy : EnemyActors)
	{
		if (!IsValid(Enemy))
		{
			continue;
		}

		if (!IsValidTargetActor(Enemy))
		{
			continue;
		}

		FVector Direction =Enemy->GetActorLocation() - ViewLocation;

		float Distance = Direction.Size();

		if (Distance > TargetRange)
		{
			continue;
		}

		Direction.Normalize();

		float Dot =FVector::DotProduct(	ViewForward,Direction);

		// 正面方向に十分近い敵だけ
		if (Dot < TargetDotThreshold)
		{
			continue;
		}

		if (Dot > BestDot)
		{
			BestDot = Dot;
			BestTarget = Enemy;
		}
	}

	return BestTarget;
}

AActor* UPlayerTargetComponent::FindNextTarget(const FVector2D& InputDirection)
{
	AActor* Owner = GetOwner();

	if (!Owner)
	{
		return nullptr;
	}

	AActor* CurrentTarget = GetCurrentTargetActor();

	if (!IsValid(CurrentTarget))
	{
		return nullptr;
	}

	APlayerController* PlayerController =Cast<APlayerController>(Owner->GetInstigatorController());

	if (!PlayerController)
	{
		return nullptr;
	}

	FVector2D InputDirection2D =InputDirection.GetSafeNormal();

	// 画面座標はYが下方向なので反転
	InputDirection2D.Y *= -1.0f;

	FVector2D CurrentTargetScreenPosition;

	if (!PlayerController->ProjectWorldLocationToScreen(CurrentTarget->GetActorLocation(),CurrentTargetScreenPosition))
	{
		return nullptr;
	}

	TArray<AActor*> EnemyActors;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName("Enemy"),EnemyActors);

	AActor* BestTarget = nullptr;

	float BestScore = -FLT_MAX;

	for (AActor* Enemy : EnemyActors)
	{
		if (!IsValid(Enemy))
		{
			continue;
		}

		// 現在のターゲットは除外
		if (Enemy == CurrentTarget)
		{
			continue;
		}

		if (!IsValidTargetActor(Enemy))
		{
			continue;
		}

		// Playerから遠すぎる敵は除外
		const float PlayerDistance =FVector::Dist(Owner->GetActorLocation(),Enemy->GetActorLocation());

		if (PlayerDistance > TargetRange)
		{
			continue;
		}

		FVector2D EnemyScreenPosition;

		if (!PlayerController->ProjectWorldLocationToScreen(Enemy->GetActorLocation(),EnemyScreenPosition))
		{
			continue;
		}

		// 現在ターゲットから見た敵の画面上の方向
		FVector2D ToEnemy =EnemyScreenPosition -CurrentTargetScreenPosition;

		if (ToEnemy.IsNearlyZero())
		{
			continue;
		}

		const float ScreenDistance = ToEnemy.Size();

		ToEnemy.Normalize();

		// 入力方向と敵の方向
		const float Dot =FVector2D::DotProduct(InputDirection2D,ToEnemy);

		// 入力方向から外れている敵は無視
		if (Dot <= 0.2f)
		{
			continue;
		}

		// 入力方向に近い敵を優先
		// 同じくらいなら画面上で近い敵を優先
		const float Score =Dot * 1000.0f -ScreenDistance;

		if (Score > BestScore)
		{
			BestScore = Score;
			BestTarget = Enemy;
		}
	}

	return BestTarget;
}

