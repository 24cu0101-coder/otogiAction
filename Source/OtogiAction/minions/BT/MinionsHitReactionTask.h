#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MinionsHitReactionTask.generated.h"

class UMinionsHitReactionComponent;
class UBlackboardComponent;

UCLASS()
class OTOGIACTION_API UMinionsHitReactionTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMinionsHitReactionTask();

protected:

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

	void OnMontageEnded(
		UAnimMontage* Montage,
		bool bInterrupted,
		UBehaviorTreeComponent* OwnerComp);

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsHitKey;

private:

	UBlackboardComponent* BBComp;
};