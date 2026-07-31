// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackBuildTask.generated.h"

class UBlackboardComponent;
/**
 * 
 */
struct FBTPlayAnimationTaskMemory
{
	TWeakObjectPtr<UAnimInstance> AnimInstance;
	TWeakObjectPtr<UAnimMontage> PlayingMontage;
};

UCLASS()
class OTOGIACTION_API UAttackBuildTask : public UBTTaskNode
{
	GENERATED_BODY()
	
	UAttackBuildTask();

public:
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTPlayAnimationTaskMemory); }

	//タスク実行時に流れる関数
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//ABPでのアニメーション変更用変数
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsAttackKey;

	// 再生したいモンタージュ
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> PlayMontage;
};
