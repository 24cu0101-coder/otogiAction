
#include "phaseManager.h"

#include "OtogiAction/minions/MinionsCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"


AphaseManager::AphaseManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// 戦闘範囲Boxを作成
	AreaBox = CreateDefaultSubobject<UBoxComponent>(
		TEXT("AreaBox")
	);

	SetRootComponent(AreaBox);

	AreaBox->SetBoxExtent(FVector(1000.0f, 1000.0f, 500.0f));

	AreaBox->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly
	);

	AreaBox->SetCollisionObjectType(ECC_WorldDynamic);

	AreaBox->SetCollisionResponseToAllChannels(ECR_Ignore);

	AreaBox->SetCollisionResponseToChannel(
		ECC_Pawn,
		ECR_Overlap
	);

	AreaBox->SetGenerateOverlapEvents(true);
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

	// BGM
	PlayPhaseBGM(NormalPhaseBGM);

	// Phase1 Minion取得
	FindPhase1Minions();
}


// Tick

void AphaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	else if (CurrentPhase == EBossPhase::Phase3)
	{
		CheckPhase3Clear();
	}
	else if (CurrentPhase == EBossPhase::BossPhase1)
	{
		CheckBossPhase1Clear();
	}
}


// =====================================
// Phase1 Minion取得
// =====================================

void AphaseManager::FindPhase1Minions()
{
	Phase1Minions.Empty();

	bHasPhase1Minions = false;

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMinionsCharacter::StaticClass(),FoundActors);

	// Box範囲内の敵だけ登録
	for (AActor* Actor : FoundActors)
	{
		AMinionsCharacter* Minion =
			Cast<AMinionsCharacter>(Actor);

		if (!IsValid(Minion))
		{
			continue;
		}

		// Actorの位置をBoxのローカル座標に変換
		FVector LocalLocation =AreaBox->GetComponentTransform().InverseTransformPosition(Minion->GetActorLocation());

		FVector BoxExtent =AreaBox->GetUnscaledBoxExtent();

		// Boxの範囲内か確認
		bool bInsideArea =
			FMath::Abs(LocalLocation.X) <= BoxExtent.X &&
			FMath::Abs(LocalLocation.Y) <= BoxExtent.Y &&
			FMath::Abs(LocalLocation.Z) <= BoxExtent.Z;

		if (bInsideArea)
		{
			Phase1Minions.Add(Minion);
		}
	}

	if (Phase1Minions.Num() > 0)
	{
		bHasPhase1Minions = true;
	}
}


// =====================================
// Phase2 Minion Spawn
// =====================================

void AphaseManager::SpawnPhase2Minions()
{
	Phase2Minions.Empty();

	bHasPhase2Minions = false;

	if (!Phase2MinionClass)
	{
		return;
	}

	for (int32 i = 0; i < Phase2MinionCount; i++)
	{
		if (!Phase2SpawnTransforms.IsValidIndex(i))
		{
			continue;
		}

		// Manager基準のローカル座標をワールド座標に変換
		FTransform SpawnTransform =Phase2SpawnTransforms[i] * GetActorTransform();

		FActorSpawnParameters SpawnParams;

		SpawnParams.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMinionsCharacter* Minion =GetWorld()->SpawnActor<AMinionsCharacter>(Phase2MinionClass,SpawnTransform,SpawnParams);

		if (IsValid(Minion))
		{
			Phase2Minions.Add(Minion);
		}
	}

	if (Phase2Minions.Num() > 0)
	{
		bHasPhase2Minions = true;
	}

}


// =====================================
// Phase3 Minion Spawn
// =====================================

void AphaseManager::SpawnPhase3Minions()
{
	Phase3Minions.Empty();

	bHasPhase3Minions = false;

	if (!Phase3MinionClass)
	{
		return;
	}

	for (int32 i = 0; i < Phase3MinionCount; i++)
	{
		if (!Phase3SpawnTransforms.IsValidIndex(i))
		{
			continue;
		}

		FTransform SpawnTransform =Phase3SpawnTransforms[i] * GetActorTransform();

		FActorSpawnParameters SpawnParams;

		SpawnParams.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AMinionsCharacter* Minion =GetWorld()->SpawnActor<AMinionsCharacter>(Phase3MinionClass,SpawnTransform,SpawnParams);

		if (IsValid(Minion))
		{
			Phase3Minions.Add(Minion);
		}
	}

	if (Phase3Minions.Num() > 0)
	{
		bHasPhase3Minions = true;
	}

}


// =====================================
// Boss Spawn
// =====================================

void AphaseManager::SpawnBossPhase1()
{
	bBossSpawnSucceeded = false;

	if (!BossClass)
	{
		return;
	}

	// Manager基準のローカル座標を変換
	FTransform SpawnTransform =BossSpawnTransform * GetActorTransform();

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	BossActor =GetWorld()->SpawnActor<AActor>(BossClass,SpawnTransform,SpawnParams);

	if (!IsValid(BossActor))
	{

		return;
	}

	bBossSpawnSucceeded = true;


	APawn* BossPawn = Cast<APawn>(BossActor);

	if (!BossPawn)
	{

		return;
	}

	AController* Controller = BossPawn->GetController();

}


// Phase1 Clear

void AphaseManager::CheckPhase1Clear()
{
	if (bIsRespawning)
	{
		return;
	}

	// 敵が登録されていなければ進まない
	if (!bHasPhase1Minions)
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


	SpawnPhase2Minions();
}


// =====================================
// Phase2 Clear
// =====================================

void AphaseManager::CheckPhase2Clear()
{
	if (bIsRespawning)
	{
		return;
	}

	if (!bHasPhase2Minions)
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

	StartPhase3();
}


// Phase2 → Phase3

void AphaseManager::StartPhase3()
{
	if (CurrentPhase != EBossPhase::Phase2)
	{
		return;
	}

	CurrentPhase = EBossPhase::Phase3;

	SpawnPhase3Minions();
}


// =====================================
// Phase3 Clear
// =====================================

void AphaseManager::CheckPhase3Clear()
{
	if (bIsRespawning)
	{
		return;
	}

	if (!bHasPhase3Minions)
	{
		return;
	}

	for (AMinionsCharacter* Minion : Phase3Minions)
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


// Phase3 → BossPhase1

void AphaseManager::StartBossPhase1()
{
	if (CurrentPhase != EBossPhase::Phase3)
	{
		return;
	}

	CurrentPhase = EBossPhase::BossPhase1;


	SpawnBossPhase1();

	if (bBossSpawnSucceeded)
	{
		PlayPhaseBGM(BossPhase1BGM);
	}
}


// =====================================
// Boss Phase1 Clear
// =====================================

void AphaseManager::CheckBossPhase1Clear()
{
	if (bIsRespawning)
	{
		return;
	}

	// Spawn失敗時は進めない
	if (!bBossSpawnSucceeded)
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

	bBossSpawnSucceeded = false;


	if (BossDefeatSE)
	{
		UGameplayStatics::PlaySound2D(this,BossDefeatSE);
	}
}


// 現在フェーズのMinionをリスポーン

void AphaseManager::RespawnCurrentPhaseMinions()
{
	// Phase1

	if (CurrentPhase == EBossPhase::Phase1)
	{
		for (AMinionsCharacter* Minion : Phase1Minions)
		{
			if (IsValid(Minion))
			{
				Minion->Destroy();
			}
		}

		Phase1Minions.Empty();

		FindPhase1Minions();


		return;
	}


	// Phase2

	if (CurrentPhase == EBossPhase::Phase2)
	{
		for (AMinionsCharacter* Minion : Phase2Minions)
		{
			if (IsValid(Minion))
			{
				Minion->Destroy();
			}
		}

		Phase2Minions.Empty();

		SpawnPhase2Minions();


		return;
	}


	// Phase3

	if (CurrentPhase == EBossPhase::Phase3)
	{
		for (AMinionsCharacter* Minion : Phase3Minions)
		{
			if (IsValid(Minion))
			{
				Minion->Destroy();
			}
		}

		Phase3Minions.Empty();

		SpawnPhase3Minions();


		return;
	}


	// BossPhase1

	if (CurrentPhase == EBossPhase::BossPhase1)
	{
		if (IsValid(BossActor))
		{
			BossActor->Destroy();
		}

		BossActor = nullptr;

		bBossSpawnSucceeded = false;

		SpawnBossPhase1();

		return;
	}
}


// =====================================
// リスポーン位置
// =====================================

FTransform AphaseManager::GetRespawnTransform() const
{
	switch (CurrentPhase)
	{
	case EBossPhase::Phase1:

		return Phase1RespawnTransform;


	case EBossPhase::Phase2:

		return Phase2RespawnTransform;


	case EBossPhase::Phase3:

		return Phase3RespawnTransform;


	case EBossPhase::BossPhase1:

		return BossPhase1RespawnTransform;


	default:

		return Phase1RespawnTransform;
	}
}


// =====================================
// リスポーン中フラグ
// =====================================

void AphaseManager::SetRespawning(bool bRespawning)
{
	bIsRespawning = bRespawning;

}


// =====================================
// BGM再生
// =====================================

void AphaseManager::PlayPhaseBGM(USoundBase* Sound)
{
	if (!Sound)
	{

		return;
	}

	UGameplayStatics::PlaySound2D(this,Sound);
}
