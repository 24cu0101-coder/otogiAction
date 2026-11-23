// Fill out your copyright notice in the Description page of Project Settings.

#include "phaseManager.h"

#include "OtogiAction/minions/MinionsCharacter.h"
#include "OtogiAction/minions/LongRangeMinion.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "TimerManager.h"


// =====================================
// 敵の死亡判定
// =====================================

static bool IsManagedEnemyDead(ACharacter* Character)
{
	if (!IsValid(Character))
	{
		return true;
	}

	// 通常Minion・盾持ちMinion
	if (AMinionsCharacter* Minion = Cast<AMinionsCharacter>(Character))
	{
		return Minion->IsDead();
	}

	// 遠距離Minion
	if (ALongRangeMinion* LongRangeMinion =
		Cast<ALongRangeMinion>(Character))
	{
		return LongRangeMinion->IsDead();
	}

	// その他のACharacter派生クラス
	// 専用の死亡フラグがない場合はDestroyされるまで生存扱い
	return Character->IsActorBeingDestroyed();
}


// =====================================
// Constructor
// =====================================

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


// =====================================
// BeginPlay
// =====================================

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


// =====================================
// Tick
// =====================================

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

	// ACharacterを継承した敵を取得
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ACharacter::StaticClass(),
		FoundActors
	);

	// Box範囲内の敵だけ登録
	for (AActor* Actor : FoundActors)
	{
		ACharacter* Character = Cast<ACharacter>(Actor);

		if (!IsValid(Character))
		{
			continue;
		}

		// プレイヤーは除外
		if (Character == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			continue;
		}

		// 管理対象の敵だけ登録
		bool bIsSupportedEnemy =
			Character->IsA(AMinionsCharacter::StaticClass()) ||
			Character->IsA(ALongRangeMinion::StaticClass());

		if (!bIsSupportedEnemy)
		{
			continue;
		}

		// Actorの位置をBoxのローカル座標に変換
		FVector LocalLocation =
			AreaBox->GetComponentTransform().InverseTransformPosition(
				Character->GetActorLocation()
			);

		FVector BoxExtent = AreaBox->GetUnscaledBoxExtent();

		// Boxの範囲内か確認
		bool bInsideArea =
			FMath::Abs(LocalLocation.X) <= BoxExtent.X &&
			FMath::Abs(LocalLocation.Y) <= BoxExtent.Y &&
			FMath::Abs(LocalLocation.Z) <= BoxExtent.Z;

		if (bInsideArea)
		{
			Phase1Minions.Add(Character);

			// 通常Minion系だけKintaroSkillを反映
			ApplyKintaroSkillToMinion(Character);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("[Phase1] Enemy Registered: %s"),
				*Character->GetName()
			);
		}
	}

	if (Phase1Minions.Num() > 0)
	{
		bHasPhase1Minions = true;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[Phase1] Registered Enemy Count: %d"),
		Phase1Minions.Num()
	);
}


// =====================================
// Phase2 Minion Spawn
// =====================================

void AphaseManager::SpawnPhase2Minions()
{
	Phase2Minions.Empty();

	bHasPhase2Minions = false;

	// 登録された敵グループを順番にスポーン
	for (const FPhaseMinionSpawnData& Group : Phase2MinionGroups)
	{
		if (!Group.MinionClass)
		{
			continue;
		}

		for (int32 i = 0; i < Group.SpawnCount; i++)
		{
			if (!Group.SpawnTransforms.IsValidIndex(i))
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("[Phase2] SpawnTransform is missing. Group=%s Index=%d"),
					*Group.MinionClass->GetName(),
					i
				);

				continue;
			}

			// Manager基準のローカル座標をワールド座標に変換
			FTransform SpawnTransform =
				Group.SpawnTransforms[i] * GetActorTransform();

			FActorSpawnParameters SpawnParams;

			SpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			ACharacter* Minion =
				GetWorld()->SpawnActor<ACharacter>(
					Group.MinionClass,
					SpawnTransform,
					SpawnParams
				);

			if (IsValid(Minion))
			{
				Phase2Minions.Add(Minion);

				// 金太郎スキル状態を反映
				ApplyKintaroSkillToMinion(Minion);

				UE_LOG(
					LogTemp,
					Warning,
					TEXT("[Phase2] Spawned: %s"),
					*Minion->GetName()
				);
			}
		}
	}

	if (Phase2Minions.Num() > 0)
	{
		bHasPhase2Minions = true;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[Phase2] Total Spawned: %d"),
		Phase2Minions.Num()
	);
}


// =====================================
// Phase3 Minion Spawn
// =====================================

void AphaseManager::SpawnPhase3Minions()
{
	Phase3Minions.Empty();

	bHasPhase3Minions = false;

	// 登録された敵グループを順番にスポーン
	for (const FPhaseMinionSpawnData& Group : Phase3MinionGroups)
	{
		if (!Group.MinionClass)
		{
			continue;
		}

		for (int32 i = 0; i < Group.SpawnCount; i++)
		{
			if (!Group.SpawnTransforms.IsValidIndex(i))
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("[Phase3] SpawnTransform is missing. Group=%s Index=%d"),
					*Group.MinionClass->GetName(),
					i
				);

				continue;
			}

			// Manager基準のローカル座標をワールド座標に変換
			FTransform SpawnTransform =
				Group.SpawnTransforms[i] * GetActorTransform();

			FActorSpawnParameters SpawnParams;

			SpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			ACharacter* Minion =
				GetWorld()->SpawnActor<ACharacter>(
					Group.MinionClass,
					SpawnTransform,
					SpawnParams
				);

			if (IsValid(Minion))
			{
				Phase3Minions.Add(Minion);

				// 金太郎スキル状態を反映
				ApplyKintaroSkillToMinion(Minion);

				UE_LOG(
					LogTemp,
					Warning,
					TEXT("[Phase3] Spawned: %s"),
					*Minion->GetName()
				);
			}
		}
	}

	if (Phase3Minions.Num() > 0)
	{
		bHasPhase3Minions = true;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[Phase3] Total Spawned: %d"),
		Phase3Minions.Num()
	);
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
	FTransform SpawnTransform =
		BossSpawnTransform * GetActorTransform();

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	BossActor = GetWorld()->SpawnActor<AActor>(
		BossClass,
		SpawnTransform,
		SpawnParams
	);

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


// =====================================
// Phase1 Clear
// =====================================

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

	for (ACharacter* Minion : Phase1Minions)
	{
		if (!IsValid(Minion))
		{
			continue;
		}

		if (!IsManagedEnemyDead(Minion))
		{
			return;
		}
	}

	StartPhase2();
}


// =====================================
// Phase1 → Phase2
// =====================================

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

	for (ACharacter* Minion : Phase2Minions)
	{
		if (!IsValid(Minion))
		{
			continue;
		}

		if (!IsManagedEnemyDead(Minion))
		{
			return;
		}
	}

	StartPhase3();
}


// =====================================
// Phase2 → Phase3
// =====================================

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

	for (ACharacter* Minion : Phase3Minions)
	{
		if (!IsValid(Minion))
		{
			continue;
		}

		if (!IsManagedEnemyDead(Minion))
		{
			return;
		}
	}

	StartBossPhase1();
}


// =====================================
// Phase3 → BossPhase1
// =====================================

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
		UGameplayStatics::PlaySound2D(this, BossDefeatSE);
	}
}


// =====================================
// 現在のフェーズのMinionをリスポーン
// =====================================

void AphaseManager::RespawnCurrentPhaseMinions()
{
	// Phase1

	if (CurrentPhase == EBossPhase::Phase1)
	{
		for (ACharacter* Minion : Phase1Minions)
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
		for (ACharacter* Minion : Phase2Minions)
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
		for (ACharacter* Minion : Phase3Minions)
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

	UGameplayStatics::PlaySound2D(this, Sound);
}


// =====================================
// 金太郎スキル状態の設定
// =====================================

void AphaseManager::SetKintaroSkillActive(bool bActive)
{
	bKintaroSkillActive = bActive;

	// プレイヤーの目印タグを確実に付け外しする
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (bActive)
		{
			PlayerPawn->Tags.AddUnique(FName("KintaroActive"));
		}
		else
		{
			PlayerPawn->Tags.Remove(FName("KintaroActive"));
		}
	}

	// 現在いるすべての通常Minion系の敵に状態を即座に反映
	TArray<AActor*> MinionActors;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AMinionsCharacter::StaticClass(),
		MinionActors
	);

	for (AActor* Actor : MinionActors)
	{
		if (AMinionsCharacter* Minion = Cast<AMinionsCharacter>(Actor))
		{
			ApplyKintaroSkillToMinion(Minion);
		}
	}

	// 既存のタイマーをクリア
	GetWorld()->GetTimerManager().ClearTimer(KintaroSkillTimerHandle);

	// 有効になった場合は15秒後に自動解除するタイマーをセット
	if (bActive)
	{
		GetWorld()->GetTimerManager().SetTimer(
			KintaroSkillTimerHandle,
			this,
			&AphaseManager::EndKintaroSkill,
			15.0f,
			false
		);
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("PhaseManager KintaroSkill = %d"),
		bKintaroSkillActive
	);
}


// =====================================
// 15秒後に自動で呼ばれる解除関数
// =====================================

void AphaseManager::EndKintaroSkill()
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Kintaro 15 sec End (by PhaseManager)")
	);

	SetKintaroSkillActive(false);
}


// =====================================
// 敵に金太郎スキル状態を反映
// =====================================

void AphaseManager::ApplyKintaroSkillToMinion(ACharacter* Character)
{
	if (!IsValid(Character))
	{
		return;
	}

	// 通常Minion系だけ対象
	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(Character);

	if (!IsValid(Minion))
	{
		return;
	}

	Minion->SetKintaroSkillActive(bKintaroSkillActive);

	// 金太郎専用敵だけ対象
	if (Minion->bKintaroOnlyEnemy)
	{
		Minion->SetCanTakeDamage(bKintaroSkillActive);
		Minion->SetCanSpawnOrb(bKintaroSkillActive);
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("[KINTARO] Apply to %s | Active=%d"),
		*Minion->GetName(),
		bKintaroSkillActive
	);
}