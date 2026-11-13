// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UChaseDecorator::UChaseDecorator()
{
	NodeName = TEXT("Is Target In Range");

	// 親クラスの BlackboardKey に対して Enum フィルタを適用
	BlackboardKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UChaseDecorator, BlackboardKey), StaticEnum<EEnemyState>());

	// デフォルトで「自身(Self)と低優先度ノード(Lower Priority)の両方」を中断可能にする
	FlowAbortMode = EBTFlowAbortMode::Both;
}

bool UChaseDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	// Blackboardから現在のStateを取得して比較
	const uint8 CurrentStateValue = BlackboardComp->GetValueAsEnum(BlackboardKey.SelectedKeyName);
	return CurrentStateValue == static_cast<uint8>(RequiredState);
}

EBlackboardNotificationResult UChaseDecorator::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	// 親クラスの処理を呼ぶことで、FlowAbortMode に基づくノード中断が自動的に走る
	return Super::OnBlackboardKeyValueChange(Blackboard, ChangedKeyID);
}
