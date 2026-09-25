#include "phaseManager.h"

#include "OtogiAction/minions/MinionsCharacter.h"
#include "Kismet/GameplayStatics.h"

AphaseManager::AphaseManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AphaseManager::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Warning,TEXT("===== BOSS PHASE MANAGER BEGIN PLAY ====="));

	// 最初はPhase1
	CurrentPhase = EBossPhase::Phase1;

	// レベル上の雑魚敵を取得
	FindPhase1Minions();
}

void AphaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Phase1なら雑魚全滅を確認

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
//第一Phase
void AphaseManager::FindPhase1Minions()
{
	Phase1Minions.Empty();

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMinionsCharacter::StaticClass(),FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AMinionsCharacter* Minion = Cast<AMinionsCharacter>(Actor);

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

//第二Phase
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
		FTransform SpawnTransform;

		// Spawn位置が設定されている場合
		if (Phase2SpawnTransforms.IsValidIndex(i))
		{
			SpawnTransform = Phase2SpawnTransforms[i];
		}
		else
		{
			UE_LOG(
				LogTemp,
				Error,
				TEXT("Phase2 SpawnTransform is missing for Minion %d"),
				i
			);

			continue;
		}

		FActorSpawnParameters SpawnParams;SpawnParams.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMinionsCharacter* Minion =GetWorld()->SpawnActor<AMinionsCharacter>(Phase2MinionClass,SpawnTransform,SpawnParams);

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

//クリアチェック１
void AphaseManager::CheckPhase1Clear()
{
	// 生きている雑魚がいるか確認
	for (AMinionsCharacter* Minion : Phase1Minions)
	{
		if (!IsValid(Minion))
		{
			continue;
		}

		// まだ生きているならPhase1継続
		if (!Minion->IsDead())
		{
			return;
		}
	}

	// 全員死亡したら
	StartPhase2();
}

//Phase2開始
void AphaseManager::StartPhase2()
{
	// 二重実行防止
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
		TEXT("====Phase 2 Start =====")
	);

	SpawnPhase2Minions();
}

//クリアチェック２
void AphaseManager::CheckPhase2Clear()
{
	// 生きている雑魚がいるか確認
	for (AMinionsCharacter* Minion : Phase2Minions)
	{
		if (!IsValid(Minion))
		{
			continue;
		}

		// まだ生きているならPhase1継続
		if (!Minion->IsDead())
		{
			return;
		}
	}

	// 全員死亡したら
	StartBossPhase1();
}


//BossPhase1開始
void AphaseManager::StartBossPhase1()
{
	// 二重実行防止
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
		TEXT("====BossPhase 1 Start =====")
	);
}

//クリアチェック３
void AphaseManager::CheckBossPhase1Clear()
{
	// Bossがまだ存在しているなら戦闘継続
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
		TEXT("===== BossPhase 1 Start =====")
	);
}