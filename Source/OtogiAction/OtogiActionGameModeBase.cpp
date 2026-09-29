// Fill out your copyright notice in the Description page of Project Settings.

#include "OtogiActionGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "OtogiAction/phase/phaseManager.h"


// コンストラクタ
AOtogiActionGameModeBase::AOtogiActionGameModeBase()
{
}


// ゲーム開始時
void AOtogiActionGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// ゲーム開始直後のプレイヤー位置を保存
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC && PC->GetPawn())
	{
		CurrentCheckpointTransform =
			PC->GetPawn()->GetActorTransform();
	}
}


// チェックポイント更新
void AOtogiActionGameModeBase::SetCheckPoint(FTransform NewTransform)
{
	CurrentCheckpointTransform = NewTransform;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Checkpoint Update")
	);
}


// プレイヤー復活
void AOtogiActionGameModeBase::RespawmPlayer(AController* TargetController)
{
	if (!TargetController)
	{
		return;
	}

	// PhaseManager取得

	AphaseManager* PhaseManager =
		Cast<AphaseManager>(
			UGameplayStatics::GetActorOfClass(
				GetWorld(),
				AphaseManager::StaticClass()
			)
		);

	// リスポーン開始

	if (PhaseManager)
	{
		PhaseManager->SetRespawning(true);
	}

	// 現在のプレイヤーを削除

	APawn* OldPawn = TargetController->GetPawn();

	if (OldPawn)
	{
		OldPawn->Destroy();
	}

	// 現在のフェーズの敵を復活

	if (PhaseManager)
	{
		PhaseManager->RespawnCurrentPhaseMinions();
	}

	// リスポーン位置

	FTransform RespawnTransform =
		CurrentCheckpointTransform;

	if (PhaseManager)
	{
		RespawnTransform =
			PhaseManager->GetRespawnTransform();

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Respawn Phase: %d"),
			(int32)PhaseManager->GetCurrentPhase()
		);
	}

	// プレイヤー生成

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (RespawnPlayerClass)
	{
		APawn* NewPawn =
			GetWorld()->SpawnActor<APawn>(
				RespawnPlayerClass,
				RespawnTransform,
				SpawnParams
			);

		if (NewPawn)
		{
			TargetController->Possess(NewPawn);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("===== NEW PLAYER RESPAWNED =====")
			);
		}
	}
	else
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("RespawnPlayerClass is NOT set!")
		);
	}
	// リスポーン終了

	if (PhaseManager)
	{
		PhaseManager->SetRespawning(false);
	}
}


// シーン遷移
void AOtogiActionGameModeBase::ChangeLevel(FName LevelName)
{
	if (!LevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(
			this,
			LevelName
		);
	}
}