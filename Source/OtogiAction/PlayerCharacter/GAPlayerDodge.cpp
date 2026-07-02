// Fill out your copyright notice in the Description page of Project Settings.


#include "GAPlayerDodge.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"


void UGAPlayerDodge::ActivateAbility(
	const FGameplayAbilitySpecHandle Dodging,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
)
{
	Super::ActivateAbility(Dodging, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//プレイヤーの情報を取得
	PlayerActor = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	//クラッシュ、バグ回避のためのチェック
	if (!PlayerActor || !DodgeMontage)
	{
		//リターン
		return;
	}

	//ドッジ開始
	DodgeStart();
}



//--------------------------------------------------------------------------------------------------------
//関数
//-----------------------------------------------------------------------------------------------------


//回避開始の処理
void UGAPlayerDodge::DodgeStart()
{
	//再生のタスク
	UAbilityTask_PlayMontageAndWait* DodgeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, NAME_None, DodgeMontage);

	//タスクがあるなら
	if (DodgeMontageTask)
	{
		//アニメーション再生
		DodgeMontageTask->ReadyForActivation();
	}

	//回避の処理
	IsDodge();
}

//回避の処理
void UGAPlayerDodge::IsDodge()
{
	//プレイヤーの情報があれば
	if (PlayerActor)
	{
		//プレイヤーの正面を取得
		FVector DodgeForward = PlayerActor->GetActorForwardVector();

		//ベクトル正規化
		DodgeForward.Normalize();

		//縦方向の動きを0に
		DodgeForward.Z = 0.f;

		//最終回避距離と方向
		FVector DodgeLocation = PlayerActor->GetActorLocation() + (DodgeForward + DodgeDistance * GetWorld()->DeltaTimeSeconds);

		PlayerActor->SetActorLocation(DodgeLocation, true);
	}
}


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
