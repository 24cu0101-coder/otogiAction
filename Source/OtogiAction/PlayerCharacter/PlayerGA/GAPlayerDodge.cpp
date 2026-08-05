// Fill out your copyright notice in the Description page of Project Settings.

#include "GAPlayerDodge.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
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

	if (!PlayerActor || !DodgeMontage)
	{
		//リターン
		return;
	}

	//タグ登録
	IsDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge"));

	StickRotate();

	//回避開始
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

	//タスクなければリターン
	if (!DodgeMontageTask) return;

	//アニメーション再生
	DodgeMontageTask->ReadyForActivation();

	//タスクがあれば
	if (DodgeMontageTask)
	{
		//終了時に自動で呼ぶ
		DodgeMontageTask->OnCancelled.AddDynamic(this, &UGAPlayerDodge::DodgeEnd);
		DodgeMontageTask->OnCompleted.AddDynamic(this, &UGAPlayerDodge::DodgeEnd);
	}

	FTimerHandle DodgeDelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DodgeDelayTimer, this, &UGAPlayerDodge::PlayDodge, DelayTiem, false);

}

//通常回避の開始
void UGAPlayerDodge::PlayDodge()
{

	//移動用の関数の引数に代入してバインド
	LocationDelegate = FTimerDelegate::CreateUObject(this, &UGAPlayerDodge::DodgeLocation, DodgeDistance);

	//回避処理開始(ほぼ毎フレーム繰り返す)
	GetWorld()->GetTimerManager().SetTimer(DodgeTimer, LocationDelegate, 0.001, true);

	JustDodgeWindow();

	//指定した時間後終了
	GetWorld()->GetTimerManager().SetTimer(EndDodgeTimer, this, &UGAPlayerDodge::DodgeEnd, DodgeTime, false);
}


//just回避受付の処理
void UGAPlayerDodge::JustDodgeWindow()
{
	UE_LOG(LogTemp, Warning, TEXT("kkkk"));

	//tagを飛ばす
	IsInvincible = FGameplayTag::RequestGameplayTag(FName("Invincible"));
	if (ASC)
	{
		//タグを付与する
		ASC->AddLooseGameplayTag(IsInvincible);
	}	
	if (IsValid(PlayerActor) && IsValid(this))
	{
		PlayerActor->OnTakeAnyDamage.AddDynamic(this, &UGAPlayerDodge::OnPlayerTakeDamage);
	}
}


void UGAPlayerDodge::OnPlayerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageCauser && DamageCauser->ActorHasTag(FName("Enemy")))
	{

		FTimerHandle EndJustDodgeTime;
		GetWorld()->GetTimerManager().SetTimer(EndJustDodgeTime, this, &UGAPlayerDodge::EndJustDodgeWindow, DodgeTime, false);
			

		//timer破棄
		GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);
		GetWorld()->GetTimerManager().ClearTimer(EndDodgeTimer);

		//
		PlayJustDodge();
	}
}


void UGAPlayerDodge::EndJustDodgeWindow()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		Player->OnTakeAnyDamage.RemoveDynamic(this, &UGAPlayerDodge::OnPlayerTakeDamage);
	}

	ASC->RemoveLooseGameplayTag(IsInvincible);

	GetWorld()->GetTimerManager().ClearTimer(EndJustDodgeTimer);
}


//ジャスト回避開始
void UGAPlayerDodge::PlayJustDodge()
{
	UE_LOG(LogTemp, Warning, TEXT("yyyyy"));

	LocationDelegate.Unbind();

	//移動用の関数の引数に代入してバインド
	LocationDelegate = FTimerDelegate::CreateUObject(this, &UGAPlayerDodge::DodgeLocation, DodgeDistance / 2);

	//回避処理開始(ほぼ毎フレーム繰り返す)
	GetWorld()->GetTimerManager().SetTimer(DodgeTimer, LocationDelegate, 0.001 / 5, true);


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
		JustDodgeMontageTask->OnCancelled.AddDynamic(this, &UGAPlayerDodge::DodgeEnd);
		JustDodgeMontageTask->OnCompleted.AddDynamic(this, &UGAPlayerDodge::DodgeEnd);
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
	GetWorld()->GetTimerManager().SetTimer(dd, this, &UGAPlayerDodge::EndJustDodge, SlowTime / 5, false);
}

//just回避終了
void UGAPlayerDodge::EndJustDodge()
{
	//ワールドの情報の取得
	UWorld* World = GetWorld();
	//ワールドが無ければ
	if (!World) return;

	DodgeEnd();

	//世界をあるべき姿に
	UGameplayStatics::SetGlobalTimeDilation(World, 1);



}

//回避時の移動
void UGAPlayerDodge::DodgeLocation(float Distance)
{
	//プレイヤーの情報と再生タスクが在れば
	if (PlayerActor)
	{

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

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		Player->OnTakeAnyDamage.RemoveDynamic(this, &UGAPlayerDodge::OnPlayerTakeDamage);
	}

	ASC->RemoveLooseGameplayTag(IsInvincible);


	//timer破棄
	GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);
	GetWorld()->GetTimerManager().ClearTimer(EndDodgeTimer);


	//モンタージュ終了させる
	DodgeMontageEnd();
}

//モンタージュ終了
void UGAPlayerDodge::DodgeMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//即座に回転させる
void UGAPlayerDodge::StickRotate()
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
