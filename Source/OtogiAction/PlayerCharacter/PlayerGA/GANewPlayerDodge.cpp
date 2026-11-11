// Fill out your copyright notice in the Description page of Project Settings.


#include "GANewPlayerDodge.h"

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

void UGANewPlayerDodge::ActivateAbility(
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

	if (!PlayerActor || !DodgeMontage) return;

	//タグ登録
	IsDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.DodgeAttack"));

	// プレイヤーの向きを即座に変更
	StickRotate();

	//回避開始
	DodgeStart();
}



//--------------------------------------------------------------------------------------------------------
//関数
//-----------------------------------------------------------------------------------------------------

//回避開始の処理
void UGANewPlayerDodge::DodgeStart()
{
	//再生のタスク
	UAbilityTask_PlayMontageAndWait* DodgeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
	(this, NAME_None, DodgeMontage);

	//タスクなければリターン
	if (!DodgeMontageTask) return;


	//タスクがあれば終了時に呼ぶ
	DodgeMontageTask->OnCancelled.AddDynamic(this, &UGANewPlayerDodge::DodgeEnd);
	DodgeMontageTask->OnCompleted.AddDynamic(this, &UGANewPlayerDodge::DodgeEnd);
	//DodgeMontageTask->OnInterrupted.AddDynamic(this, &UGANewPlayerDodge::DodgeEnd);


	//アニメーション再生
	DodgeMontageTask->ReadyForActivation();


	// just回避のタグを受け取る
	m_justDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.JustDodge"));


	PlayDodge();
}

//通常回避の開始
void UGANewPlayerDodge::PlayDodge()
{
	// 無敵tagを飛ばす(現在は必要なし)
	IsInvincibleTag = FGameplayTag::RequestGameplayTag(FName("Invincible"));

	// アビリティシステムコンポーネントがあるなら
	if (ASC)
	{
		//タグを付与する
		ASC->AddLooseGameplayTag(IsInvincibleTag);
	}

	// ディレイタイムが設定されているなら
	if (DelayTiem > 0)
	{
		// 回避にディレイをかけてstart
		FTimerHandle DodgeDelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DodgeDelayTimer, this, &UGANewPlayerDodge::DodgeLocationStart, DelayTiem, false);
	}
	// いないなら即座に開始
	else
	{
		//回避開始
		//DodgeLocationStart();
	}


	// タグがあればjust回避を受け付ける
	//JustDodgeWindow();

	//指定した時間後終了
	GetWorld()->GetTimerManager().SetTimer(EndDodgeTimer, this, &UGANewPlayerDodge::DodgeEnd, DodgeTime, false);
}




void UGANewPlayerDodge::DodgeLocationStart()
{
	if (!PlayerActor)return;

	//正面を取得
	FVector DodgeForward = PlayerActor->GetActorForwardVector();
	DodgeForward.Z = 0.0f;
	DodgeForward.Normalize();

	//float w = (3 / 4);

	//float DodgeTime2 = DodgeTime * w;

	// 移動速度
	float Speed = DodgeDistance / DodgeTime;

	// ルートモーションタスク生成
	m_moveTask =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this, NAME_None, DodgeForward, Speed, DodgeTime, false, DedgeSpeedCurve,
			ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, 0.0f, false);


	// ルートモーションタスクがあるなら
	if (m_moveTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("【GANewPlayerDodge.cpp】:DodgeLocationStart"));


		m_moveTask->ReadyForActivation();
	}


	////移動用の関数の引数に代入してバインド
	//LocationDelegate = FTimerDelegate::CreateUObject(this, &UGAPlayerDodge::DodgeLocation, DodgeDistance);

	////回避処理開始(ほぼ毎フレーム繰り返す)
	//GetWorld()->GetTimerManager().SetTimer(DodgeTimer, LocationDelegate, 0.001, true);
}

//回避時の移動
void UGANewPlayerDodge::DodgeLocation(float Distance)
{
	////プレイヤーの情報と再生タスクが在れば
	//if (PlayerActor)
	//{

	//	//プレイヤーの正面を取得
	//	FVector DodgeForward = PlayerActor->GetActorForwardVector();

	//	//縦方向の動きを0に
	//	DodgeForward.Z = 0.f;

	//	//ベクトル正規化
	//	DodgeForward.Normalize();

	//	//最終回避距離と方向(なんか正規化)
	//	FVector DodgeLocation = PlayerActor->GetActorLocation() + (DodgeForward * DodgeDistance * GetWorld()->DeltaTimeSeconds);

	//	//プレイヤーを移動
	//	PlayerActor->SetActorLocation(DodgeLocation, true);
	//}
}


//回避終了時の処理
void UGANewPlayerDodge::DodgeEnd()
{


	ASC->RemoveLooseGameplayTag(IsInvincibleTag);


	//timer破棄
	GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);
	GetWorld()->GetTimerManager().ClearTimer(EndDodgeTimer);


	//モンタージュ終了させる
	DodgeMontageEnd();
}

//モンタージュ終了
void UGANewPlayerDodge::DodgeMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


//即座に回転させる
void UGANewPlayerDodge::StickRotate()
{
	APlayerController* PC = Cast<APlayerController>(PlayerActor->GetController());
	if (!PC) return;

	// スティックの入力状態を取得
	float X = PC->GetInputAnalogKeyState(EKeys::Gamepad_LeftX);
	float Y = PC->GetInputAnalogKeyState(EKeys::Gamepad_LeftY);

	// キーボード（WASD）フォールバック
	if (FMath::IsNearlyZero(X) && FMath::IsNearlyZero(Y))
	{
		if (PC->IsInputKeyDown(EKeys::D)) X += 1.f;
		if (PC->IsInputKeyDown(EKeys::A)) X -= 1.f;
		if (PC->IsInputKeyDown(EKeys::W)) Y += 1.f;
		if (PC->IsInputKeyDown(EKeys::S)) Y -= 1.f;
	}

	FVector2D MovementVector(X, Y);

	if (!MovementVector.IsNearlyZero())
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FRotator CameraYawRotation(0.f, CameraRotation.Yaw, 0.f);

		FVector CameraForward = CameraYawRotation.Vector(); // カメラの正面方向
		FVector CameraRight = FRotationMatrix(CameraYawRotation).GetScaledAxis(EAxis::Y); // カメラの右方向

		FVector WorldDirection = (CameraForward * Y) + (CameraRight * X);
		WorldDirection.Z = 0.f;

		FRotator TargetRotation = WorldDirection.Rotation();

		PlayerActor->SetActorRotation(TargetRotation, ETeleportType::TeleportPhysics);

		if (UCharacterMovementComponent* MoveComp = PlayerActor->GetCharacterMovement())
		{
			MoveComp->UpdatedComponent->SetWorldRotation(TargetRotation);
		}
	}
}



//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
