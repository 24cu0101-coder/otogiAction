// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BossBaseAttackComponent.h"
#include "BossPushAttackComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UBossPushAttackComponent : public UBossBaseAttackComponent
{
	GENERATED_BODY()
	
public:
	UBossPushAttackComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --- ベースクラスのオーバーライド ---
	virtual void StartAttack_Implementation() override;
	virtual void EndAttack_Implementation() override;

	// --- NotifyStateから呼ばれる関数 ---
	/** ジャンプ移動の開始 */
	UFUNCTION(BlueprintCallable, Category = "Attack|Jump")
	void BeginJumpMovement(AActor* TargetActor, float Duration, float ArcHeight = 350.0f);

	/** ジャンプ移動の終了（着地） */
	UFUNCTION(BlueprintCallable, Category = "Attack|Jump")
	void EndJumpMovement();

protected:
	/** ターゲットと重ならないよう手前に着地させる距離（cm） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Jump")
	float LandingOffsetDistance = 150.0f;

	/** モンタージュ再生速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Animation")
	float MontagePlayRate = 1.0f;

private:
	bool bIsJumping = false;
	float ElapsedTime = 0.0f;
	float JumpDuration = 1.0f;
	float JumpArcHeight = 350.0f;

	FVector StartLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY()
	TObjectPtr<ACharacter> CachedOwnerCharacter;
};
