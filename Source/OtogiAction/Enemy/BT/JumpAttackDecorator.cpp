// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpAttackDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UJumpAttackDecorator::UJumpAttackDecorator()
{
    NodeName = "Can JumpAttack";
}

bool UJumpAttackDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (!BBComp) return false;

    float TargetTime = BBComp->GetValueAsFloat(TargetTimeKey.SelectedKeyName);
    float CurrentTime = GetWorld()->GetTimeSeconds();

    UE_LOG(LogTemp, Log, TEXT("TargetTime: %f, CurrentTime: %f"), TargetTime, CurrentTime);

    // タイマーが未設定（初期値 0.0f）なら条件不成立
    if (TargetTime <= 0.0f)
    {
        return false;
    }

    // 現在のゲーム時間が、セットされた「攻撃可能になる時間」を超えているかを判定
    return GetWorld()->GetTimeSeconds() >= TargetTime;
}
