#include "phaseManager.h"

#include "OtogiAction/minions/MinionsCharacter.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"


AphaseManager::AphaseManager()
{
	PrimaryActorTick.bCanEverTick = true;
}


// BeginPlay

void AphaseManager::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("===== BOSS PHASE MANAGER BEGIN PLAY =====")
	);

	// 最初はPhase1
	CurrentPhase = EBossPhase::Phase1;

	// レベルに配置されているPhase1 Minionを取得
	FindPhase1Minions();
}


// Tick

void AphaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// リスポーン中はフェーズチェックしない
	if (bIsRespawning)
	{
		return;
	}

	if (CurrentPhase == EBossPhase::Phase1)
	{
		CheckPhase1Clear();
	}
	else if (CurrentPhase == EBossPhase::Phase2)
	{
		CheckPhase2Clear();
	}
	else if (CurrentPhase == EBossPhase::BossPhase1)
	{
		CheckBossPhase1Clear();
	}
}


// Phase1 Minion取得

void AphaseManager::FindPhase1Minions()
{
	Phase1Minions.Empty();

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AMinionsCharacter::StaticClass(),
		FoundActors
	);

	for (AActor* Actor : FoundActors)
	{
		AMinionsCharacter* Minion =
			Cast<AMinionsCharacter>(Actor);

		if (Minion)
		{
			Phase1Minions.Add(Minion);
		}
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("BossPhaseManager: Phase1 Minions = %d"),
		Phase1Minions.Num()
	);
}


// Phase2 Minion Spawn

void AphaseManager::SpawnPhase2Minions()
{
	Phase2Minions.Empty();

	if (!Phase2MinionClass)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Phase2MinionClass is not set!")
		);

		return;
	}

	for (int32 i = 0; i < Phase2MinionCount; i++)
	{
		if (!Phase2SpawnTransforms.IsValidIndex(i))
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("Phase2 SpawnTransform is missing for Minion %d"),
				i
			);

			continue;
		}

		FTransform SpawnTransform =
			Phase2SpawnTransforms[i];

		FActorSpawnParameters SpawnParams;

		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMinionsCharacter* Minion =
			GetWorld()->SpawnActor<AMinionsCharacter>(
				Phase2MinionClass,
				SpawnTransform,
				SpawnParams
			);

		if (Minion)
		{
			Phase2Minions.Add(Minion);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Phase2 Minion Spawned: %d"),
				i
			);
		}
	}
}


// Boss Spawn

void AphaseManager::SpawnBossPhase1()
{
	if (!BossClass)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("BossClass is not set!")
		);

		return;
	}

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	BossActor =
		GetWorld()->SpawnActor<AActor>(
			BossClass,
			BossSpawnTransform,
			SpawnParams
		);

	if (!BossActor)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("===== BOSS SPAWN FAILED =====")
		);

		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("===== BOSS SPAWNED =====")
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Boss Location = %s"),
		*BossActor->GetActorLocation().ToString()
	);

	// BossがPawnならController確認
	APawn* BossPawn =
		Cast<APawn>(BossActor);

	if (!BossPawn)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("BossActor is not a Pawn!")
		);

		return;
	}

	AController* Controller =
		BossPawn->GetController();

	if (Controller)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("===== BOSS CONTROLLER FOUND =====")
		);
	}
	else
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("===== BOSS CONTROLLER NOT FOUND =====")
		);
	}
}


// Phase1 Clear

void AphaseManager::CheckPhase1Clear()
{
	if (bIsRespawning)
	{
		return;
	}

	for (AMinionsCharacter* Minion : Phase1Minions)
	{
		if (!IsValid(Minion))
		{
			continue;
		}

		if (!Minion->IsDead())
		{
			return;
		}
	}

	StartPhase2();
}


// Phase1 → Phase2

void AphaseManager::StartPhase2()
{
	if (CurrentPhase != EBossPhase::Phase1)
	{
		return;
	}

	CurrentPhase = EBossPhase::Phase2;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("===== PHASE 1 CLEAR =====")
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("==== Phase 2 Start =====")
	);

	SpawnPhase2Minions();
}


// Phase2 Clear

void AphaseManager::CheckPhase2Clear()
{
	if (bIsRespawning)
	{
		return;
	}

	for (AMinionsCharacter* Minion : Phase2Minions)
	{
		if (!IsValid(Minion))
		{
			continue;
		}

		if (!Minion->IsDead())
		{
			return;
		}
	}

	StartBossPhase1();
}


// Phase2 → BossPhase1

void AphaseManager::StartBossPhase1()
{
	if (CurrentPhase != EBossPhase::Phase2)
	{
		return;
	}

	CurrentPhase = EBossPhase::BossPhase1;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("===== PHASE 2 CLEAR =====")
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("==== BossPhase 1 Start =====")
	);

	SpawnBossPhase1();
}


// Boss Phase1 Clear

void AphaseManager::CheckBossPhase1Clear()
{
	if (bIsRespawning)
	{
		return;
	}

	// Bossが存在しているなら戦闘継続
	if (IsValid(BossActor))
	{
		return;
	}

	// BossがDestroyされた
	CurrentPhase = EBossPhase::BossPhase2;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("===== BOSS PHASE 1 CLEAR =====")
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("===== BOSS PHASE 2 =====")
	);
}


// 現在フェーズのMinionをリスポーン

void AphaseManager::RespawnCurrentPhaseMinions()
{
	// Phase1

	if (CurrentPhase == EBossPhase::Phase1)
	{
		// Phase1のMinionを削除
		for (AMinionsCharacter* Minion : Phase1Minions)
		{
			if (IsValid(Minion))
			{
				Minion->Destroy();
			}
		}

		Phase1Minions.Empty();
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Phase1 Respawn")
		);

		return;
	}


	// Phase2

	if (CurrentPhase == EBossPhase::Phase2)
	{
		// 現在のPhase2 Minionを削除
		for (AMinionsCharacter* Minion : Phase2Minions)
		{
			if (IsValid(Minion))
			{
				Minion->Destroy();
			}
		}

		Phase2Minions.Empty();

		// Phase2 Minion再生成
		SpawnPhase2Minions();

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Phase2 Respawn")
		);

		return;
	}


	// BossPhase1

	if (CurrentPhase == EBossPhase::BossPhase1)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Boss Phase Respawn")
		);

		return;
	}
}

// リスポーン位置

FTransform AphaseManager::GetRespawnTransform() const
{
	switch (CurrentPhase)
	{
	case EBossPhase::Phase1:

		return Phase1RespawnTransform;


	case EBossPhase::Phase2:

		return Phase2RespawnTransform;


	case EBossPhase::BossPhase1:

		return BossPhase1RespawnTransform;


	default:

		return Phase1RespawnTransform;
	}
}


// リスポーン中フラグ

void AphaseManager::SetRespawning(bool bRespawning)
{
	bIsRespawning = bRespawning;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("PhaseManager Respawning = %s"),
		bIsRespawning
		? TEXT("TRUE")
		: TEXT("FALSE")
	);
}