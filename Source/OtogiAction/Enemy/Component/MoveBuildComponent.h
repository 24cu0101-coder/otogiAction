// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "MoveBuildComponent.generated.h"

/**
 * 
 */

UCLASS()
class OTOGIACTION_API UMoveBuildComponent : public UEnemyAttackBaseComponent
{
	GENERATED_BODY()
	

public:
	UMoveBuildComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//stateをセットする関数
	virtual void SetEnemyState()override;

	//追跡開始処理
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	virtual bool StartTracking();

	//追跡を更新する関数
	void UpdateTrackingStatus();

	//追跡終了処理
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	virtual void FinishTracking();

	bool IsTracking()const { return bIsTracking; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class AAIController> CachedAIController;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> CachedMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tracking|Speed")
	float NormalWalkSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracking|State")
	bool bIsTracking = false;
};
