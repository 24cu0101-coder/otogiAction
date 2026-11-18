
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "phaseManager.generated.h"

class UBoxComponent;
class AMinionsCharacter;


UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1		UMETA(DisplayName = "Phase1-Minion"),
	Phase2		UMETA(DisplayName = "Phase2-Minion"),
	Phase3		UMETA(DisplayName = "Phase3-Minion"),
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


	// =====================================
	// 戦闘エリア設定
	// =====================================

	// 敵を管理する範囲
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Battle Area"
	)
	TObjectPtr<UBoxComponent> AreaBox;


	// =====================================
	// フェーズ
	// =====================================

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


	// =====================================
	// リスポーン
	// =====================================

	FTransform GetRespawnTransform() const;

	UFUNCTION(BlueprintCallable)
	void SetRespawning(bool bRespawning);

	void RespawnCurrentPhaseMinions();


	// =====================================
	// Phase1
	// =====================================

	UPROPERTY()
	TArray<AMinionsCharacter*> Phase1Minions;

	void FindPhase1Minions();

	void CheckPhase1Clear();


	// =====================================
	// Phase2
	// =====================================

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase2"
	)
	TSubclassOf<AMinionsCharacter> Phase2MinionClass;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase2"
	)
	int32 Phase2MinionCount = 3;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase2"
	)
	TArray<FTransform> Phase2SpawnTransforms;

	UPROPERTY()
	TArray<AMinionsCharacter*> Phase2Minions;

	void CheckPhase2Clear();

	void SpawnPhase2Minions();


	// =====================================
	// Phase3
	// =====================================

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase3"
	)
	TSubclassOf<AMinionsCharacter> Phase3MinionClass;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase3"
	)
	int32 Phase3MinionCount = 3;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Phase3"
	)
	TArray<FTransform> Phase3SpawnTransforms;

	UPROPERTY()
	TArray<AMinionsCharacter*> Phase3Minions;

	void CheckPhase3Clear();

	void SpawnPhase3Minions();


	// =====================================
	// Boss Phase1
	// =====================================

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Boss"
	)
	TSubclassOf<AActor> BossClass;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Boss"
	)
	FTransform BossSpawnTransform;

	UPROPERTY()
	AActor* BossActor = nullptr;

	void SpawnBossPhase1();

	void CheckBossPhase1Clear();


	// =====================================
	// フェーズ移行
	// =====================================

	void StartPhase2();

	void StartPhase3();

	void StartBossPhase1();


	// =====================================
	// リスポーン位置
	// =====================================

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
	FTransform Phase3RespawnTransform;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Boss Phase|Respawn"
	)
	FTransform BossPhase1RespawnTransform;


	// =====================================
	// フェーズBGM・SE
	// =====================================

	UPROPERTY(EditAnywhere, Category = "Phase Sound")
	TObjectPtr<USoundBase> NormalPhaseBGM;

	UPROPERTY(EditAnywhere, Category = "Phase Sound")
	TObjectPtr<USoundBase> BossPhase1BGM;

	UPROPERTY(EditAnywhere, Category = "Phase Sound")
	TObjectPtr<USoundBase> BossDefeatSE;

	void PlayPhaseBGM(USoundBase* Sound);


private:

	// リスポーン中フラグ
	bool bIsRespawning = false;

	// Phase1の敵が存在するか
	bool bHasPhase1Minions = false;

	// Phase2の敵が存在するか
	bool bHasPhase2Minions = false;

	// Phase3の敵が存在するか
	bool bHasPhase3Minions = false;

	//Bossがスポーンしてるかどうか
	bool bBossSpawnSucceeded = false;

};
