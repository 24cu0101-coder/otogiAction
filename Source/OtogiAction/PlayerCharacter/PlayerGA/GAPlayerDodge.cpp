// Fill out your copyright notice in the Description page of Project Settings.


#include "GAPlayerDodge.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"

//コンストラクタ
UGAPlayerDodge::UGAPlayerDodge()
{

	////実行中のタグを登録
	//AbilityTags.AddTag(IsDodgeTag);
}

void UGAPlayerDodge::ActivateAbility(
	const FGameplayAbilitySpecHandle Dodging,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
)
{
	Super::ActivateAbility(Dodging, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティ取得
	ASC = GetAbilitySystemComponentFromActorInfo();

	//プレイヤーの情報を取得
	PlayerActor = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	//クラッシュ、バグ回避のためのチェック
<<<<<<< HEAD:Source/OtogiAction/PlayerCharacter/GAPlayerDodge.cpp
	if (!PlayerActor || !DodgeMontage)
=======
	if (!PlayerActor || !DodgeMontage )
>>>>>>> origin/master:Source/OtogiAction/PlayerCharacter/PlayerGA/GAPlayerDodge.cpp
	{
		//リターン
		return;
	}


	//タグ登録
	IsDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge"));	


	//回避開始までのタイマー
	FTimerHandle DodgeDelayTimer;
	//エディタで設定した時間待ってから回避実行
	GetWorld()->GetTimerManager().SetTimer(DodgeDelayTimer, this, &UGAPlayerDodge::DodgeStart, DelayTiem, false);

}



//--------------------------------------------------------------------------------------------------------
//関数
//-----------------------------------------------------------------------------------------------------



//回避開始の処理
void UGAPlayerDodge::DodgeStart()
{
<<<<<<< HEAD:Source/OtogiAction/PlayerCharacter/GAPlayerDodge.cpp
=======
	//世界からtimerをもらう
	//回避処理開始
	GetWorld()->GetTimerManager().SetTimer(DodgeTimer, this, &UGAPlayerDodge::IsDodge, 0.001f, true);

	//数秒後処理終了
	FTimerHandle EndDodgeTimer;
	//0.2秒後終了
	GetWorld()->GetTimerManager().SetTimer(EndDodgeTimer, this, &UGAPlayerDodge::DodgeEnd, DodgeTime, false);	
}



//回避の処理
void UGAPlayerDodge::IsDodge()
{

>>>>>>> origin/master:Source/OtogiAction/PlayerCharacter/PlayerGA/GAPlayerDodge.cpp
	//再生のタスク
	UAbilityTask_PlayMontageAndWait* DodgeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, NAME_None, DodgeMontage);

	//プレイヤーの情報と再生タスクが在れば
	if (PlayerActor && DodgeMontageTask)
	{
		//アニメーション再生
		DodgeMontageTask->ReadyForActivation();


		//プレイヤーの正面を取得
		FVector DodgeForward = PlayerActor->GetActorForwardVector();

		//縦方向の動きを0に
		DodgeForward.Z = 0.f;

		//ベクトル正規化
		DodgeForward.Normalize();

		//最終回避距離と方向(なんか正規化)
		FVector DodgeLocation = PlayerActor->GetActorLocation() + (DodgeForward * DodgeDistance * GetWorld()->DeltaTimeSeconds);

		//プレイヤーを移動
		PlayerActor->SetActorLocation(DodgeLocation, true);
	}
}



//回避終了時の処理
void UGAPlayerDodge::DodgeEnd()
{
	//timer破棄
	GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);

	//モンタージュ終了させる
	DodgeMontageEnd();
}



//モンタージュ終了
void UGAPlayerDodge::DodgeMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
 
 
 
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
