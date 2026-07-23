// Fill out your copyright notice in the Description page of Project Settings.


#include "GASkillUrasima.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"



UGASkillUrasima::UGASkillUrasima()
{

}
void UGASkillUrasima::ActivateAbility(
	const FGameplayAbilitySpecHandle TheWorld,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
)
{


	Super::ActivateAbility(TheWorld, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	UrasimaASC = GetAbilitySystemComponentFromActorInfo();

	//アビリティシステムコンポーネントと、ゲームエフェクトがあるなら
	if (UrasimaASC && TimeStopEffect)
	{
		FGameplayEffectContextHandle Context = UrasimaASC->MakeEffectContext();
		Context.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = UrasimaASC->MakeOutgoingSpec(TimeStopEffect, 1.0f, Context);
		if (SpecHandle.IsValid())
		{
			// 付与したエフェクトのハンドル（手形）を保存しておく
			ActiveEffectHandle = UrasimaASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			if (ActiveEffectHandle.IsValid())
			{
				FOnActiveGameplayEffectRemoved_Info* RemoveDelegate = UrasimaASC->OnGameplayEffectRemoved_InfoDelegate(ActiveEffectHandle);
				if (RemoveDelegate)
				{
					RemoveDelegate->AddUObject(this, &UGASkillUrasima::TheWorldDelegate);
				}

			}

			//全てのアクターをスキャンする
			ScanActor();
		}
	}

	//どちらかでもなかったら
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		//リターン
		return;
	}

	////N秒後
	//UAbilityTask_WaitDelay* RestartTask = UAbilityTask_WaitDelay::WaitDelay(this, StopTime);

	////明日が欲しいタスクがあるなら
	//if (RestartTask)
	//{
	//	RestartTask->OnFinish.AddDynamic(this, &UGASkillTamatebakoTheWorld::TimeRestart);
	//	RestartTask->ReadyForActivation();
	//}	
}

//アクターをscanする
void UGASkillUrasima::ScanActor()
{

	//アクター取得
	OwnerActor = GetAvatarActorFromActorInfo();

	//アクターがなければリターン
	if (!OwnerActor) return;

	if (APawn* OwnerPawn = Cast<APawn>(OwnerActor))
	{
		OwnerController = OwnerPawn->GetController();
	}

	//アクターが無いとリターン
	if (!OwnerController) return;

	//全てのアクターを探す	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);


	//それぞれのアクターの時間を止める
	for (AActor* Actor : FoundActors)
	{
		// プレイヤー自身とプレイヤーコントローラーは除外
		if (Actor == OwnerActor || Actor == OwnerController)
		{
			continue;
		}

		//アクターの時間を停止
		Actor->CustomTimeDilation = 0.0f;


		if (ACharacter* Char = Cast<ACharacter>(Actor))
		{
			if (USkeletalMeshComponent* Mesh = Char->GetMesh())
			{
				Mesh->GlobalAnimRateScale = 0.0f;
			}
		}
	}
}


//時は動き出す
void UGASkillUrasima::TimeRestart()
{
	//それぞれのアクターの時間を止める
	for (AActor* Actor : FoundActors)
	{
		// プレイヤー自身とプレイヤーコントローラーは除外
		if (Actor == OwnerActor || Actor == OwnerController)
		{
			continue;
		}
		//アクターの時間を停止
		Actor->CustomTimeDilation = 1.0f;

		if (ACharacter* Char = Cast<ACharacter>(Actor))
		{
			if (USkeletalMeshComponent* Mesh = Char->GetMesh())
			{
				Mesh->GlobalAnimRateScale = 1.0f;
			}
		}
	}
	FoundActors.Empty();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGASkillUrasima::TheWorldDelegate(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	TimeRestart();

}

void UGASkillUrasima::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (UrasimaASC && ActiveEffectHandle.IsValid())
	{
		//effectを消す
		UrasimaASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
	}
	//親クラスのEndAbilityを呼ぶ
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


