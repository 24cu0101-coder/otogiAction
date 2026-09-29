#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OtogiAction/minions/MinionsCharacter.h"
#include "OtogiActionGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class OTOGIACTION_API AOtogiActionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	// コンストラクタ
	AOtogiActionGameModeBase();

	// チェックポイントの更新
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void SetCheckPoint(FTransform NewTransform);

	// プレイヤーの復活処理
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RespawmPlayer(AController* TargetController);

	// シーン遷移
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void ChangeLevel(FName LevelName);

protected:

	virtual void BeginPlay() override;


private:

	// 現在の復活ポイント座標
	UPROPERTY(VisibleAnywhere, Category = "GameMode")
	FTransform CurrentCheckpointTransform;

	// リスポーン時に使用するプレイヤー
	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<APawn> RespawnPlayerClass;
};