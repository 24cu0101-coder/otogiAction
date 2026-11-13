// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../PlayerCharacter/PlayerCharacter.h"

#include "BossChaseComponent.generated.h"

class ABossCharacter;
class ABossAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UBossChaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossChaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 追跡機能の有効化 / 無効化切り替え用
	UFUNCTION(BlueprintCallable, Category = "AI|Tracking")
	void SetTrackingEnabled(bool bEnable);

private:
	//追跡対象
	UPROPERTY(VisibleAnywhere, Category = "AI|Tracking")
	TObjectPtr<APlayerCharacter> TargetActor;

	//所有しているボスのAIControllerキャッシュ
	UPROPERTY()
	TObjectPtr<ABossAIController> CachedAIController;

	//追跡するかどうかのフラグ
	UPROPERTY(EditAnywhere, Category = "AI|Tracking")
	bool bIsTrackingEnable = false;

	//プレイヤーの位置を再計算する秒数
	UPROPERTY()
	float UpdateInterval = 0.2f;

	//タイマー管理用
	float TimerAccumulator = 0.0f;

	//プレイヤーを検索してセットする処理
	void SetTargetPlayer();

	//実際の移動命令を実行する関数
	void TrackingPlayer();
};
