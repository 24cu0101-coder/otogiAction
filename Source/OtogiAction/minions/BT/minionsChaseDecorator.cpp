#include "minionsChaseDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UminionsChaseDecorator::UminionsChaseDecorator()
{
    //ノード名
    NodeName = "Minions Chase Decorator";

    bAllowAbortNone = true;
    bAllowAbortLowerPri = false;
    bAllowAbortChildNodes = false;

    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UminionsChaseDecorator, BlackboardKey));
}

bool UminionsChaseDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    // ブラックボードから TargetLocation の座標を取得
    FVector TargetLoc = BlackboardComp->GetValueAsVector(BlackboardKey.SelectedKeyName);

    // 座標が (0,0,0) でなければtrueを返す
    return !TargetLoc.IsZero();
}