// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UtilityAIComponent.generated.h"

class UEnemyAttackBaseComponent;
class AAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUtilityAIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//エディターで設定可能な行動リスト
	UPROPERTY(EditAnywhere, Instanced, Category = "UtilityAI")
	TArray<TObjectPtr<UEnemyAttackBaseComponent>> Actions;

	//定期的に評価を実行するタイマー
	UPROPERTY(EditAnywhere, Category = "UtilityAI")
	float EvaluaionInterval = 0.5f;

private:

	//行動を評価する関数
	void EvaluateAndExecute();

	//このクラスで敵AIを参照するための変数
	UPROPERTY()
	TObjectPtr<AAIController> OwnerAIController;

	//計算した評価値を元に実行する行動を呼び出す変数
	UPROPERTY()
	TObjectPtr<UEnemyAttackBaseComponent> CurrentBestAction;

	//タイマー用ハンドル
	FTimerHandle TimerHandleEvaluate;
};
