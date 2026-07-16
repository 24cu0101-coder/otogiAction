// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeExceededDetector.h"
#include "BehaviorTree/BlackboardComponent.h"

UTimeExceededDetector::UTimeExceededDetector()
{
    NodeName = "Is Time Elapsed";

    //// このノードを「条件が変わった瞬間に自動でタスクをキャンセル/実行するトリガー」として使いたい場合
    //bAllowNodeActivation = true;
}

bool UTimeExceededDetector::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp) return false;

    float TargetTime = BBComp->GetValueAsFloat(TargetTimeKey.SelectedKeyName);

    // タイマーが未設定（初期値 0.0f）なら条件不成立
    if (TargetTime <= 0.0f)
    {
        return false;
    }

    // 現在のゲーム時間が、セットされた「攻撃可能になる時間」を超えているかを判定
    return GetWorld()->GetTimeSeconds() >= TargetTime;
}
