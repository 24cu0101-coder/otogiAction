// Fill out your copyright notice in the Description page of Project Settings.

#include "BossEnemyAttack01GA.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

void UBossEnemyAttack01GA::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // スーパークラスの処理を呼ぶ（BPのActivateAbilityイベントを呼び出すためにも必要）
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    //// 1. コミット（コストとクールダウンのチェック＆消費）
    //if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    //{
    //    // コストが足りない、クールダウン中などの場合は即終了
    //    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    //    return;
    //}

    // 2. ここにC++でのメインロジックを書く（タスクの生成など）

    // ※非同期タスクを使わない単純な即時処理なら、ここでEndAbilityを呼ぶ
    // EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UBossEnemyAttack01GA::TriggerBossAttack()
{
    // 所有者（アクター）からASCを取得する
    AActor* Owner = GetOwningActorFromActorInfo();
    if (!Owner) return;

    UAbilitySystemComponent* ASC = Owner->FindComponentByClass<UAbilitySystemComponent>();
    if (!ASC) return;

    // 先ほどのループ処理
    for (TSubclassOf<UGameplayAbility> AbilityClass : AttackAbility)
    {
        if (!AbilityClass) continue;

        // 1. アビリティをまだ授与（登録）していない場合、授与する
        // ※通常はゲーム開始時(BeginPlay)などにまとめて授与するのが理想ですが、簡易的な例としてここでチェックします。
        FGameplayAbilitySpecHandle SpecHandle;
        
        // すでに同じアビリティを持っているか探す
        FGameplayAbilitySpec* ExistingSpec = ASC->FindAbilitySpecFromClass(AbilityClass);
        if (ExistingSpec)
        {
            SpecHandle = ExistingSpec->Handle;
        }
        else
        {
            // 持っていなければ新しく授与してハンドルを取得
            SpecHandle = ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
        }

        // 2. 授与されたハンドルを使って、アビリティを発動（実行）する
        if (SpecHandle.IsValid())
        {
            ASC->TryActivateAbility(SpecHandle);
        }
    }
}
