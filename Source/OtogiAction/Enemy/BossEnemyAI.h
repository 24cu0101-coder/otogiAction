// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BossEnemyCharacter.h"

#include "BossEnemyAI.generated.h"

//前方宣言
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

//namespace BBKeys
//{
//	const FName PlayerActor = TEXT("PlayerActor");
//	const FName DistanceToPlayer = TEXT("DistanceToPlayer");
//	const FName AttackRange = TEXT("AttackRange");
//	const FName MonitoringRange = TEXT("MonitoringRange");
//	const FName WaitTime = TEXT("WaitTime");
//	const FName IsPlayerInSight = TEXT("IsPlayerInSight");
//	const FName CanAttack = TEXT("CanAttack");
//}

UCLASS()
class OTOGIACTION_API ABossEnemyAI : public AAIController
{
	GENERATED_BODY()

public:
	ABossEnemyAI();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

protected:
	//// AIPerceptionコンポーネント
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	//UAIPerceptionComponent* AIPerceptionComp;

	//// 視覚の設定用オブジェクト
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	//UAISenseConfig_Sight* SightConfig;

	// ターゲットが更新されたときに呼ばれる関数（イベントバインド用）
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	//UBehaviorTree* BehaviorTreeAsset;

	//ACharacter* TargetActor;

	//TObjectPtr<ABossEnemyCharacter> EnemyCharacter;
private:
	////コンポーネントの保持
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Param", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

};
