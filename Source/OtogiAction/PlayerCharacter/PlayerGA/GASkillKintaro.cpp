// Fill out your copyright notice in the Description page of Project Settings.


#include "GASkillKintaro.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "OtogiAction/Phase/phaseManager.h"


UGASkillKintaro::UGASkillKintaro()
{
	//tagの追加
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Attacking.Skill")));
}


//abilityの発動
void UGASkillKintaro::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// PhaseManagerに金太郎スキル有効状態を通知（タグ付与・全敵への適用・15秒タイマー管理はPhaseManagerが実行）
	AphaseManager* PhaseManager = Cast<AphaseManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AphaseManager::StaticClass())
	);

	if (PhaseManager)
	{
		PhaseManager->SetKintaroSkillActive(true);
	}

	//モンタージュタスクの作成
	if (KintaroMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("KintaroAction"), KintaroMontage, 1.0f);

		if (PlayMontageTask)
		{
			// アニメーション完了、中断、キャンセルの各種イベントをバインド
			PlayMontageTask->OnCompleted.AddDynamic(this, &UGASkillKintaro::AbilityFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UGASkillKintaro::AbilityFinished);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UGASkillKintaro::AbilityFinished);

			// タスクをアクティブにして再生開始
			PlayMontageTask->ReadyForActivation();
		}
		else
		{
			// タスク作成に失敗した場合は、アニメーション無しで即バフ付与へ進む
			AbilityFinished();
		}
	}
	else
	{
		// アニメーションが設定されていない場合も直接バフ付与へ進む
		AbilityFinished();
	}
}


//金太郎状態終了
void UGASkillKintaro::AbilityFinished()
{
	//金太郎の身体強化バフ（GE）をプレイヤー自身に適用する
	if (KintaroEffectClass)
	{
		// エフェクトのスペックを生成
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(KintaroEffectClass, GetAbilityLevel());

		if (SpecHandle.IsValid())
		{
			// 自分自身（Owner）に適用
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}