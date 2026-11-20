// Fill out your copyright notice in the Description page of Project Settings.


#include "BossComboAttackDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UBossComboAttackDecorator::UBossComboAttackDecorator()
{
	// Behavior Treeエディタ上に表示される名前
	NodeName = TEXT("Can Combo Attack");

	// BlackboardKey に EEnemyState（Enum）型のキーだけを選択できるように制限
	BlackboardKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBossComboAttackDecorator, BlackboardKey), StaticEnum<EEnemyState>());

	// 状態が変わった瞬間に、現在実行中のタスクを強制中断（Abort）する設定
	FlowAbortMode = EBTFlowAbortMode::Both;
}

bool UBossComboAttackDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	// Blackboardから現在のAIStateを取得し、RequiredState（Strafe）と一致するか判定
	const uint8 CurrentStateValue = BlackboardComp->GetValueAsEnum(BlackboardKey.SelectedKeyName);
	return CurrentStateValue == static_cast<uint8>(RequiredState);
}

EBlackboardNotificationResult UBossComboAttackDecorator::OnBlackboardKeyValueChange(
	const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	// 親クラス（UBTDecorator_BlackboardBase）の処理を呼ぶことで、
	// 値が変わった瞬間に CalculateRawConditionValue が再評価され、FlowAbortMode に従って中断処理が走る
	return Super::OnBlackboardKeyValueChange(Blackboard, ChangedKeyID);
}
