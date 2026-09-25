
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "phaseManager.generated.h"


UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1			UMETA(DisplayName = "Phase1-Minion"),
	Phase2			UMETA(DisplayName = "Phase2-Minion"),
	BossPhase1		UMETA(DisplayName = "BossPhase1"),
	BossPhase2		UMETA(DisplayName = "BossPhase2"),
	Clear			UMETA(DisplayName = "Clear"),
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

	// 現在のフェーズ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Phase", meta = (AllowPrivateAccess = "true"))
	EBossPhase CurrentPhase = EBossPhase::Phase1;

	// Phase1で使用する雑魚敵
	UPROPERTY()
	TArray<class AMinionsCharacter*> Phase1Minions;

	// Phase1にいる雑魚敵を取得
	void FindPhase1Minions();

	// Phase1の雑魚敵が全滅したか確認
	void CheckPhase1Clear();

	// Phase2でSpawnする雑魚敵のクラス
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Phase|Phase2")
	TSubclassOf<AMinionsCharacter> Phase2MinionClass;

	// Phase2のMinionを何体Spawnするか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Phase|Phase2")
	int32 Phase2MinionCount = 3;

	// Phase2のMinion出現位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Phase|Phase2")
	TArray<FTransform> Phase2SpawnTransforms;

	// Phase2でSpawnした雑魚敵
	UPROPERTY()
	TArray<class AMinionsCharacter*> Phase2Minions;

	// Phase2の雑魚敵が全滅したか確認
	void CheckPhase2Clear();

	// Phase2の敵をSpawn
	void SpawnPhase2Minions();


	// BossPhase1でSpawnするBoss
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Phase|Boss")
	TSubclassOf<AActor> BossClass;

	// Bossの出現位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Phase|Boss")
	FTransform BossSpawnTransform;

	// SpawnしたBoss
	UPROPERTY()
	AActor* BossActor = nullptr;

	// BossをSpawn
	void SpawnBossPhase1();

	//Bossが死んだか確認
	void CheckBossPhase1Clear();


	// Phase1終了->Phase2移行
	void StartPhase2();

	// Phase2終了->BossPhase移行
	void StartBossPhase1();

};
