// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "JumpAttackTask.generated.h"

class UEnemyAttackBaseComponent;
class UBlackboardComponent;
class ABossEnemyCharacter;

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UJumpAttackTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UJumpAttackTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 実行したい攻撃コンポーネントのクラスをBT上で指定
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<UEnemyAttackBaseComponent> AttackClass;

	// Blackboardのキーを指定するためのプロパティ
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TimerKey;

	// Blackboardのキーを指定するためのプロパティ
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CanJumpAttackkey;

	// Blackboardのキーを指定するためのプロパティ
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PlayerActorKey;

	// 攻撃タスクの header (.h) に追加
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// 攻撃終了時に呼ばれるコールバック関数
	UFUNCTION()
	void OnAttackCompleted(bool bSuccess);

	// 現在実行中のタスクコンポーネントを保持
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	UBlackboardComponent* BBComp;

	ABossEnemyCharacter* EnemyCharacter;
	//MaxWalkSpeedの初期値を設定
	float DefaultSpeed;
};
