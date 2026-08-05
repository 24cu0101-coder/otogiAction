// Fill out your copyright notice in the Description page of Project Settings.


#include "GASkillKintaro.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "OtogiAction/minions/MinionsCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


UGASkillKintaro::UGASkillKintaro()
{
	//tagの追加
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Attacking.Skill")));
}


//abilityの発動
void UGASkillKintaro::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	TArray<AActor*> MinionActors;


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinionsCharacter::StaticClass(), MinionActors);

	for (AActor* Actor : MinionActors)
	{
		AMinionsCharacter* Minion = Cast<AMinionsCharacter>(Actor);


		if (Minion)
		{
			if (Minion->bKintaroOnlyEnemy)
			{
				Minion->SetCanSpawnOrb(true);

				UE_LOG(LogTemp, Warning,
					TEXT("SetCanSpawnOrb true : %s"),
					*Minion->GetName());
			}
		}
	}


	//金太郎状態中のみ敵のOrb生成を許可
	GetWorld()->GetTimerManager().SetTimer(KintaroTimerHandle, this, &UGASkillKintaro::DisableKintaroEnemyOrb, 15.0f, false);


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




//金太郎専用敵のOrb生成を解除
void UGASkillKintaro::DisableKintaroEnemyOrb()
{
	UE_LOG(LogTemp, Warning, TEXT("DisableKintaroEnemyOrb Called"));


	TArray<AActor*> MinionActors;


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinionsCharacter::StaticClass(), MinionActors);



	for (AActor* Actor : MinionActors)
	{

		AMinionsCharacter* Minion = Cast<AMinionsCharacter>(Actor);


		if (Minion && Minion->bKintaroOnlyEnemy)
		{

			UE_LOG(LogTemp, Warning,
				TEXT("%s Disable Orb"),
				*Minion->GetName());


			Minion->SetCanSpawnOrb(false);


			UE_LOG(LogTemp, Warning,
				TEXT("CanSpawnOrb = %d"),
				Minion->bCanSpawnOrb);

		}

	}

}