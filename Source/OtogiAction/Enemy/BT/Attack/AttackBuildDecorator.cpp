// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBuildDecorator.h"
#include "../../EnemyStateSubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UAttackBuildDecorator::UAttackBuildDecorator()
{
	NodeName = TEXT("Is Enemy AttackBuild");

	//デコレーターがBlackboardの値を監視して自動で中断（Abort）を実行できるようにする設定
	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;

}

bool UAttackBuildDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GI = World->GetGameInstance())
        {
            //GameInstance を取得して Subsystem を呼び出す
            if (UEnemyStateSubsystem* StateSubsystem = GI->GetSubsystem<UEnemyStateSubsystem>())
            {
                // 状態の参照
                if (StateSubsystem->CurrentState == EEnemyState::AttackBuild)
                {
                    return true;
                }
            }
        }
    }

	return false;
}
