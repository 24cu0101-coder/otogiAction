// Fill out your copyright notice in the Description page of Project Settings.

#include "GANormalAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"

//コンストラクタ
UGANormalAttack::UGANormalAttack():
	NAttackIndex(0),
	NAttackASC(nullptr),
	Count(0),
	Countlimit(3)

{

}


void UGANormalAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle NormalAttack,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
) 
{
	Super::ActivateAbility(NormalAttack, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティシステムコンポーネントを取得
	NAttackASC = GetAbilitySystemComponentFromActorInfo();
			

	//モンタージュのloopをcountするためのタグ
	FGameplayTag TargetTag = FGameplayTag::RequestGameplayTag(FName("LoopCounter"));

	//四段目のイベント
	UAbilityTask_WaitGameplayEvent* SheathingEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TargetTag,
		nullptr,
		false,
		false
	);

	//アニメーションloopの際のイベントのタスクがあれば
	if (SheathingEvent)
	{
		SheathingEvent->EventReceived.AddDynamic(this, &UGANormalAttack::LoopCount);

		//UE_LOG(LogTemp, Warning, TEXT("yy"));

		SheathingEvent->ReadyForActivation();

	}


	//アニメーションを再生する関数を呼ぶ
	PlayNAttackMontage();
}

//アニメーション再生
void UGANormalAttack::PlayNAttackMontage()
{

	if (NAttackMontages)
	{

		//アニメーション再生タスク
		UAbilityTask_PlayMontageAndWait* NAttackMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
			(this, NAME_None, NAttackMontages);

		//タスクがあれば
		if (NAttackMontageTask)
		{
			//終了時に自動で呼ぶ
			if (!NAttackMontageTask->OnCancelled.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnCancelled.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
			if (!NAttackMontageTask->OnCompleted.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnCompleted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
			if (!NAttackMontageTask->OnInterrupted.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnInterrupted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
		}

		//アニメーション再生
		NAttackMontageTask->ReadyForActivation();

	}
}

//モンタージュ終了時に呼び出す
void UGANormalAttack::NAttackMontageEnd()
{

	NAttackAbilityEnd();
}

//アビリティ終了を終了する
void UGANormalAttack::NAttackAbilityEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//loopのcountと脱出
void UGANormalAttack::LoopCount(FGameplayEventData Payload)
{
	//loopのたびに加算
	++Count;

	//loopの限界を超えたら
	if (Countlimit < Count)
	{
		//キャラクター取得
		ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());

		//キャラクターがあれば
		if (Character)
		{
			//メッシュを取得
			USkeletalMeshComponent* Mesh = Character->GetMesh();
			//メッシュがあればq
			if (Mesh)
			{
				//現在のアニメーションを取得
				UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

				//原罪再生しているアニメーションがあれば
				if (AnimInstance)
				{
					//loopのシーケンスを抜ける
					AnimInstance->Montage_SetNextSection(FName("roop1"), FName("roop2"), CurrentMontage);

					//countを初期化
					Count = 0;
				}
			}
		}
	}
}
