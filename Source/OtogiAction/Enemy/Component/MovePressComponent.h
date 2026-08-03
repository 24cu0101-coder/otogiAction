// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "MovePressComponent.generated.h"

/**
 * 
 */

UCLASS()
class OTOGIACTION_API UMovePressComponent : public UEnemyAttackBaseComponent
{
	GENERATED_BODY()
	
public:
	UMovePressComponent();

	//stateをセットする関数
	virtual void SetEnemyState()override;

	/** ダッシュ追跡開始処理 */
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	virtual bool StartDashTracking();

	/** ダッシュ追跡終了処理 */
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	virtual void FinishDashTracking();
	bool IsDashTracking() const { return bIsDashTracking; }
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tracking|Speed")
	float DashSpeed = 750.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tracking|State")
	bool bIsDashTracking = false;
};
