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

	//アビリティシステムコンポーネントがあれば
	if (NAttackASC)
	{
		//アニメーションを再生する関数を呼ぶ
		PlayNAttackMontage();


		//アニメーションのループをカウントする処理を実行(対応アニメーションの場合だけ実行するように修正予定  本アニメーション次第では廃止)
		SetLoopCountTask();

		//アニメーションをスローにする処理を実行(本アニメーション次第では廃止)
		//SloawMontageTaskSet();
	}
	//アビリティシステムコンポーネントが無ければ
	else
	{
		//return
		return;
	}
}

//アニメーション再生
void UGANormalAttack::PlayNAttackMontage()
{
	//通常攻撃のモンタージュがあれば
	if (NAttackMontages)
	{
		//ループのカウントを0に
		Count = 0;

		//アニメーション再生タスク
		NAttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
			(this, NAME_None, NAttackMontages);

		//タスクがあれば
		if (NAttackMontageTask)
		{
			//終了時に自動で呼ぶ
			if (!NAttackMontageTask->OnCancelled.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnCompleted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
				NAttackMontageTask->OnInterrupted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
				NAttackMontageTask->OnCancelled.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
			if (!NAttackMontageTask->OnCompleted.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnCompleted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
				NAttackMontageTask->OnInterrupted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
				NAttackMontageTask->OnCancelled.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
			if (!NAttackMontageTask->OnInterrupted.IsAlreadyBound(this, &UGANormalAttack::NAttackMontageEnd))
			{
				NAttackMontageTask->OnCompleted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
				NAttackMontageTask->OnInterrupted.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
				NAttackMontageTask->OnCancelled.AddDynamic(this, &UGANormalAttack::NAttackMontageEnd);
			}
		}
		//アニメーション再生
		NAttackMontageTask->ReadyForActivation();

	}
}

//loopをcountするタスクを設定する関数
void UGANormalAttack::SetLoopCountTask()
{
	//モンタージュのloopをcountするためのタグ
	FGameplayTag TargetTag = FGameplayTag::RequestGameplayTag(FName("LoopCounter"));

	//四段目のイベント
	SheathingEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TargetTag,
		nullptr,
		false,
		false
	);

	//アニメーションloopの際のイベントのタスクがあれば
	if (SheathingEvent)
	{
		//ループをカウントする処理を実行するように設定
		SheathingEvent->EventReceived.AddDynamic(this, &UGANormalAttack::LoopCount);

		//UE_LOG(LogTemp, Warning, TEXT("yy"));

		//実行
		SheathingEvent->ReadyForActivation();

	}
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
					SheathingEvent->EndTask();
				}
			}
		}
	}
}

//モンタージュの速度を遅くするイベントタスクを設定する関数
//void UGANormalAttack::SloawMontageTaskSet()
//{
//	//モンタージュの速度を遅くするためのタグ
//	FGameplayTag SlowTag = FGameplayTag::RequestGameplayTag(FName("SlowMontage"));
//
//	//slowになるイベント
//	SlowMontageEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
//		this,
//		SlowTag,
//		nullptr,
//		false,
//		false
//	);
//
//	//アニメーションloopの際のイベントのタスクがあれば
//	if (SlowMontageEvent)
//	{
//
//		//SlowMontageEvent->EventReceived.AddDynamic(this, &UGANormalAttack::SloawMontage);
//
//
//		//SlowMontageEvent->ReadyForActivation();
//
//	}
//}

//モンタージュの速度を下げる
void UGANormalAttack::SloawMontage(FGameplayEventData Payload)
{

	if (ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SAttackAnimInstance = Char->GetMesh()->GetAnimInstance())
		{
			//通常攻撃のアニメーションを再生するタスクがすでになければ
			if (!NAttackMontageTask)
			{					
				//returnする	
				return;
			}

			//アニメーションを止める
			if (SAttackAnimInstance) {
				SAttackAnimInstance->Montage_SetPlayRate(NAttackMontages, 0.01f);
			}
		}
	}

	//再生速度を下げるタスクがあるなら
	if (SlowMontageEvent)
	{
		//タスクを終わらせる
		SlowMontageEvent->EndTask();
	}
}

void UGANormalAttack::EventTaskEnd(UAbilityTask_WaitGameplayEvent* EventTask)
{
	EventTask->EndTask();
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

