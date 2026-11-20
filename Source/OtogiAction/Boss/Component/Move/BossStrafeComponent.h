// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossStrafeComponent.generated.h"

class ABossAIController;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UBossStrafeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossStrafeComponent();

	// Taskから呼び出す有効化 / 無効化関数
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetStrafeEnabled(bool bEnable);

	// 現在の状態取得
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool GetStrafeFlg() const { return bIsStrafeEnabled; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 左右移動の速度倍率
	UPROPERTY(EditAnywhere, Category = "Strafe")
	float StrafeSpeedScale = 0.6f;

	// 左右の方向をランダムに切り替える間隔（秒）
	UPROPERTY(EditAnywhere, Category = "Strafe")
	float ChangeDirectionInterval = 2.5f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<ABossAIController> CachedAIController;

	UPROPERTY()
	TObjectPtr<APawn> CachedOwnerPawn;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> TargetActor;

	bool bIsStrafeEnabled = false;
	float StrafeDirection = 1.0f; // -1.0f = 左, 1.0f = 右
	float DirectionTimer = 0.0f;

	void SetTargetPlayer();
	void PickRandomDirection();
};
