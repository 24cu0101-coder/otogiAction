#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "minionsChaseDecorator.generated.h"

UCLASS()
class OTOGIACTION_API UminionsChaseDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    UminionsChaseDecorator();

protected:
    // ツリーの条件判定を行うコア関数
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

    //TargetLocationを設定する
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BlackboardKey;
};