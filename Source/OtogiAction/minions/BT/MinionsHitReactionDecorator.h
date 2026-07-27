#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MinionsHitReactionDecorator.generated.h"

UCLASS()
class OTOGIACTION_API UMinionsHitReactionDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UMinionsHitReactionDecorator();

protected:

	// 条件判定
	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) const override;

	// BlackboardのIsHitキー
	UPROPERTY(EditAnywhere, Category = "AI|Blackboard")
	FBlackboardKeySelector IsHitKey;
};