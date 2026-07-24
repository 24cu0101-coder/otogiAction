// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "HitReactionTask.generated.h"

class UBossEnemyHitReactionComponent;

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UHitReactionTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UHitReactionTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 再生完了時に呼ばれる関数
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp);

	// 実行したいHitコンポーネントのクラスをBT上で指定
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<UBossEnemyHitReactionComponent> HitReactionClass;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsHitKey;

private:
	UBlackboardComponent* BBComp;

};
