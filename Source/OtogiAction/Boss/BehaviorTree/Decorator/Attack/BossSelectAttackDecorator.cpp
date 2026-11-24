// Fill out your copyright notice in the Description page of Project Settings.


#include "BossSelectAttackDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UBossSelectAttackDecorator::UBossSelectAttackDecorator()
{
	NodeName = TEXT("Check Enemy State");

	// BlackboardKey に EEnemyState（Enum）型のキーだけを選択できるように制限
	BlackboardKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBossSelectAttackDecorator, BlackboardKey), StaticEnum<EEnemyState>());

	// 行動完了後に SelectNextAction タスクで状態を切り替えるため、デフォルトは None が安全
	FlowAbortMode = EBTFlowAbortMode::None;
}

bool UBossSelectAttackDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	const uint8 CurrentStateValue = BlackboardComp->GetValueAsEnum(BlackboardKey.SelectedKeyName);
	return CurrentStateValue == static_cast<uint8>(RequiredState);
}

EBlackboardNotificationResult UBossSelectAttackDecorator::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	return Super::OnBlackboardKeyValueChange(Blackboard, ChangedKeyID);
}

FString UBossSelectAttackDecorator::GetStaticDescription() const
{
	// Enumの表示名を取得（例: "NormalAttack"）
	const UEnum* EnumPtr = StaticEnum<EEnemyState>();
	const FString StateName = EnumPtr
		? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(RequiredState)).ToString()
		: TEXT("Invalid");

	return FString::Printf(TEXT("%s == %s"), *BlackboardKey.SelectedKeyName.ToString(), *StateName);
}
