// Fill out your copyright notice in the Description page of Project Settings.


#include "GAPlayerJustDodge.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "OtogiAction/GameFlow/Actor/TutorialActor.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/PlayerDodgeComponent.h"


UGAPlayerJustDodge::UGAPlayerJustDodge()
{

}

void UGAPlayerJustDodge::ActivateAbility(
	const FGameplayAbilitySpecHandle Dodging,
	const FGameplayAbilityActorInfo* playerActorInfo, 
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
)
{
	Super::ActivateAbility(Dodging, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティ取得
	m_asc = GetAbilitySystemComponentFromActorInfo();

	//プレイヤーの情報を取得
	m_playerActor = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	m_playerDogeComp = m_playerActor->FindComponentByClass<UPlayerDodgeComponent>();

	//回避開始
	PlayJustDodge();
}

// ジャスト回避開始
void UGAPlayerJustDodge::PlayJustDodge()
{
	JustDodgeLocation();

	JustDodgeRotate(m_playerDogeComp->GetEnemyRote());

	//カウンター攻撃受付のタグを飛ばす
	m_justDogeAttack = FGameplayTag::RequestGameplayTag(FName("CounterAttack"));
	if (m_asc)
	{
		//タグを付与する
		m_asc->AddLooseGameplayTag(m_justDogeAttack);
	}

	//再生のタスク
	UAbilityTask_PlayMontageAndWait* JustDodgeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, NAME_None, JustDodgeMontage);

	//タスクなければリターン
	if (!JustDodgeMontageTask) return;

	//アニメーション再生
	JustDodgeMontageTask->ReadyForActivation();

	//タスクがあれば
	if (JustDodgeMontageTask)
	{
		//終了時に自動で呼ぶ
		JustDodgeMontageTask->OnCancelled.AddDynamic(this, &UGAPlayerJustDodge::EndJustDodge);
		JustDodgeMontageTask->OnCompleted.AddDynamic(this, &UGAPlayerJustDodge::EndJustDodge);
	}


	//ワールドの情報の取得
	UWorld* World = GetWorld();
	//ワールドが無ければ
	if (!World) return;

	//世界をスローに
	UGameplayStatics::SetGlobalTimeDilation(World, SlowMagnification);


	//数秒後処理終了
	FTimerHandle dd;
	//指定したフレーム後にjust回避受付終了
	GetWorld()->GetTimerManager().SetTimer(dd, this, &UGAPlayerJustDodge::EndJustDodge, SlowTime / 5, false);

}

void UGAPlayerJustDodge::JustDodgeLocation()
{
	//プレイヤーの情報と再生タスクが在れば
	if (m_playerActor)
	{

		//プレイヤーの正面を取得
		FVector DodgeForward = m_playerActor->GetActorForwardVector();

		//縦方向の動きを0に
		DodgeForward.Z = 0.f;

		//ベクトル正規化
		DodgeForward.Normalize();

		//最終回避距離と方向(なんか正規化)
		FVector DodgeLocation = m_playerActor->GetActorLocation() + (DodgeForward * JustDodgeDistance);

		//プレイヤーを移動
		m_playerActor->SetActorLocation(DodgeLocation, true);
	}

}

void UGAPlayerJustDodge::JustDodgeRotate(FVector Angle)
{
	if (UCharacterMovementComponent* MoveComp = m_playerActor->GetCharacterMovement())
	{
		FRotator rotate = Angle.Rotation();
		m_playerActor->SetActorRotation(rotate, ETeleportType::TeleportPhysics);
	}
}

void UGAPlayerJustDodge::EndJustDodge()
{
	//ワールドの情報の取得
	UWorld* World = GetWorld();
	//ワールドが無ければ
	if (!World) return;

	//カウンター攻撃のタグを破棄
	m_asc->RemoveLooseGameplayTag(m_justDogeAttack);

	//// ジャスト回避攻撃のタグ削除
	//m_asc->RemoveLooseGameplayTag(m_justDodgeAttack);

	//m_justDodgeWind = true;

	//EndJustDodgeWindow();

	

	//世界をあるべき姿に
	UGameplayStatics::SetGlobalTimeDilation(World, 1);

	


	//モンタージュ終了させる
	JustDodgeMontageEnd();

}

//モンタージュ終了
void UGAPlayerJustDodge::JustDodgeMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


//--------------------------------------------------------------------------------------------------------
//関数
//-----------------------------------------------------------------------------------------------------







