#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LongRangeMinion.generated.h"

class AArrowActor;
class USceneComponent;

UCLASS()
class OTOGIACTION_API ALongRangeMinion : public ACharacter
{
	GENERATED_BODY()

public:
	ALongRangeMinion();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	// 矢の発射位置（エディタ上で弓の先端位置に調整可能）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* MuzzlePoint;

	// 発射する矢のクラス（エディタ上でBP化したArrowActorを指定）
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AArrowActor> ArrowClass;

	// 攻撃間隔（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireInterval = 2.0f;

	// 索敵・射程距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 2500.0f;

	// 回転の滑らかさ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RotationInterpSpeed = 5.0f;

private:
	FTimerHandle FireTimerHandle;

	// 矢を撃つ処理
	void ShootArrow();
};