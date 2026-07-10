// Fill out your copyright notice in the Description page of Project Settings.


#include "GASkillKintaro.h"
#include "GameFramework/Character.h"

UGASkillKintaro::UGASkillKintaro()
{
	//tagの追加
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Attacking.Skill")));


	//斧アクターの初期化
	SpawnAxActor = nullptr;
}

//abilityの発動
void UGASkillKintaro::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//ownerキャラクターがいるか検索
	ACharacter* OwnerChar = (CurrentActorInfo) ? Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;

	//キャラクターの正面空間に独立してスポーンさせる処理
	if (OwnerChar && AxActorClass && !SpawnAxActor)
	{
		//キャラクターの現在位置、向いている方向（回転）を取得
		FVector CharLocation = OwnerChar->GetActorLocation();
		FRotator CharRotation = OwnerChar->GetActorRotation();

		//キャラクターの正面方向基準で、Offsetの分だけ位置をずらす
			FVector SpawnLocation = CharLocation
			+ (OwnerChar->GetActorForwardVector() * SpawnOffset.X) // 前方への距離
			+ (OwnerChar->GetActorRightVector() * SpawnOffset.Y)   // 左右へのズレ（基本0）
			+ (FVector::UpVector * SpawnOffset.Z);                // 高さの調整

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerChar;
		SpawnParams.Instigator = OwnerChar;

		// 計算した位置と向きで、完全に単独（独立）したアクターとしてスポーン！
		SpawnAxActor = GetWorld()->SpawnActor<AActor>(AxActorClass, SpawnLocation, CharRotation, SpawnParams);
	}

	if (!KintaroMontage)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, KintaroMontage, 1.0f);
	
	if (MontageTask)
	{
		// animationが終了したときとEndAbilityに移行
		MontageTask->OnCompleted.AddDynamic(this, &UGASkillKintaro::AbilityFinished);
		MontageTask->OnBlendOut.AddDynamic(this, &UGASkillKintaro::AbilityFinished);
		MontageTask->OnInterrupted.AddDynamic(this, &UGASkillKintaro::AbilityFinished);
		MontageTask->OnCancelled.AddDynamic(this, &UGASkillKintaro::AbilityFinished);

		// タスクを起動（これでアニメーションが再生されます）
		MontageTask->ReadyForActivation();
	}
}

void UGASkillKintaro::AbilityFinished()
{
	if (SpawnAxActor)
	{
		SpawnAxActor->Destroy();
		SpawnAxActor = nullptr; // メモリを綺麗にする
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

