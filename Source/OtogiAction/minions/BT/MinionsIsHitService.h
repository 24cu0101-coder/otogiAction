#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MinionsIsHitService.generated.h"

UCLASS()
class OTOGIACTION_API UMinionsIsHitService : public UBTService
{
	GENERATED_BODY()

public:
	UMinionsIsHitService();

protected:

	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsHitKey;
};