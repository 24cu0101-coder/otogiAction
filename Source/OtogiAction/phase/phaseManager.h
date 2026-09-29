#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "phaseManager.generated.h"


UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1		UMETA(DisplayName = "Phase1-Minion"),
	Phase2		UMETA(DisplayName = "Phase2-Minion"),
	BossPhase1	UMETA(DisplayName = "BossPhase1"),
	BossPhase2	UMETA(DisplayName = "BossPhase2"),
	Clear		UMETA(DisplayName = "Clear"),
};


UCLASS()
class OTOGIACTION_API AphaseManager : public AActor
{
	GENERATED_BODY()

public:

	AphaseManager();


protected:

	virtual void BeginPlay() override;


public:

	virtual void Tick(float DeltaTime) override;


	// フェーズ

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase",
		meta = (AllowPrivateAccess = "true")
	)
	EBossPhase CurrentPhase = EBossPhase::Phase1;


	// 現在のフェーズ取得
	EBossPhase GetCurrentPhase() const
	{
		return CurrentPhase;
	}


	// リスポーン

	// 現在フェーズに応じたリスポーン位置
	FTransform GetRespawnTransform() const;

	// リスポーン中か設定
	UFUNCTION(BlueprintCallable)
	void SetRespawning(bool bRespawning);

	// 現在フェーズのMinionを再生成
	void RespawnCurrentPhaseMinions();


	// Phase1

	// Phase1で使用する雑魚敵
	UPROPERTY()
	TArray<class AMinionsCharacter*> Phase1Minions;

	// Phase1の雑魚敵を取得
	void FindPhase1Minions();

	// Phase1の雑魚敵が全滅したか確認
	void CheckPhase1Clear();


	// Phase2

	// Phase2でSpawnする雑魚敵
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase2"
	)
	TSubclassOf<AMinionsCharacter> Phase2MinionClass;

	// Phase2のMinion数
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase2"
	)
	int32 Phase2MinionCount = 3;

	// Phase2のMinion出現位置
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase2"
	)
	TArray<FTransform> Phase2SpawnTransforms;

	// Phase2でSpawnしたMinion
	UPROPERTY()
	TArray<class AMinionsCharacter*> Phase2Minions;

	// Phase2の雑魚敵が全滅したか確認
	void CheckPhase2Clear();

	// Phase2の敵をSpawn
	void SpawnPhase2Minions();


	// Boss Phase1

	// Bossクラス
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Boss"
	)
	TSubclassOf<AActor> BossClass;

	// Boss出現位置
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Boss"
	)
	FTransform BossSpawnTransform;

	// SpawnしたBoss
	UPROPERTY()
	AActor* BossActor = nullptr;

	// Boss Spawn
	void SpawnBossPhase1();

	// Boss死亡確認
	void CheckBossPhase1Clear();


	// フェーズ移行

	// Phase1 → Phase2
	void StartPhase2();

	// Phase2 → BossPhase1
	void StartBossPhase1();


	// リスポーン位置

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Respawn"
	)
	FTransform Phase1RespawnTransform;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Respawn"
	)
	FTransform Phase2RespawnTransform;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Respawn"
	)
	FTransform BossPhase1RespawnTransform;


private:

	// リスポーン中フラグ
	bool bIsRespawning = false;
};