// Fill out your copyright notice in the Description page of Project Settings.

#include "GAPlayerDodge.h"
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
	IsDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.NormalDodge"));

	m_justDodgeWind = true;

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

	
	//タスクがあれば終了時に呼ぶ
	DodgeMontageTask->OnCancelled.AddDynamic(this, &UGAPlayerDodge::DodgeEnd);
	DodgeMontageTask->OnCompleted.AddDynamic(this, &UGAPlayerDodge::DodgeEnd);
	

	//アニメーション再生
	DodgeMontageTask->ReadyForActivation();

	
	// just回避のタグを受け取る
	m_justDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.JustDodge"));

	m_justDodgeAttack = FGameplayTag::RequestGameplayTag("IsDodge.DodgeAttack");


	PlayDodge();
}

//通常回避の開始
void UGAPlayerDodge::PlayDodge()
{
	// 無敵tagを飛ばす(現在は必要なし)
	IsInvincibleTag = FGameplayTag::RequestGameplayTag(FName("Invincible"));
	if (ASC)
	{
		//タグを付与する
		ASC->AddLooseGameplayTag(IsInvincibleTag);
	}



	if (DelayTiem > 0)
	{
		// 回避にディレイをかけてstart
		FTimerHandle DodgeDelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DodgeDelayTimer, this, &UGAPlayerDodge::DodgeLocationStart, DelayTiem, false);
	}

	else
	{
		//回避開始
		DodgeLocationStart();
	}


	// タグがあればjust回避を受け付ける
	JustDodgeWindow();

	//指定した時間後終了
	GetWorld()->GetTimerManager().SetTimer(EndDodgeTimer, this, &UGAPlayerDodge::DodgeEnd, DodgeTime, false);
}


//just回避受付の処理
void UGAPlayerDodge::JustDodgeWindow()
{	


	// just回避終了時のタグが消える瞬間を検知するタスク
	UAbilityTask_WaitGameplayTagRemoved* WaitTagremovedTask =
		UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, m_justDodgeTag);

	if (WaitTagremovedTask)
	{

		WaitTagremovedTask->Removed.AddDynamic(this, &UGAPlayerDodge::EndJustDodgeWindow);
		WaitTagremovedTask->ReadyForActivation();
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
		// 攻撃してきた敵の方向を求める
		m_justDodgeRelativeAngle = GetEnemyAngleVector(DamageCauser);

		//// 指定した時間後ジャスト回避終了
		//FTimerHandle EndJustDodgeTime;
		//GetWorld()->GetTimerManager().SetTimer(EndJustDodgeTime, this, &UGAPlayerDodge::EndJustDodgeWindow, m_justDodgeTime, false);


		//timer破棄
		GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);
		GetWorld()->GetTimerManager().ClearTimer(EndDodgeTimer);



		//ジャスト回避
		if (ASC->HasMatchingGameplayTag(m_justDodgeTag)) PlayJustDodge();
	}
}


void UGAPlayerDodge::EndJustDodgeWindow()
{
	// just回避受付のflagがあれば
	if (m_justDodgeWind)
	{
		// イベント解除
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo()))
		{
			Player->OnTakeAnyDamage.RemoveDynamic(this, &UGAPlayerDodge::OnPlayerTakeDamage);
		}


	// タグ消滅
	//ASC->RemoveLooseGameplayTag(IsInvincibleTag);

	GetWorld()->GetTimerManager().ClearTimer(m_endJustDodgeTimer);
	}
}


//ジャスト回避開始
void UGAPlayerDodge::PlayJustDodge()
{
	// チュートリアルを取得
	ATutorialActor* TutorialManager =
		Cast<ATutorialActor>(
			UGameplayStatics::GetActorOfClass(
				GetWorld(),
				ATutorialActor::StaticClass()
			)
		);

	// ジャスト回避のチュートリアルをクリア
	if (TutorialManager)
	{
		TutorialManager->CompleteStep(7);
	}

	//if (ASC)return;

	ASC->AddLooseGameplayTag(m_justDodgeAttack);


	if (ACharacter* Player = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SAttackAnimInstance = Player->GetMesh()->GetAnimInstance())
		{
			//通常攻撃のアニメーションを再生するタスクがすでになければ
			if (!DodgeMontage)
			{
				//returnする	
				return;
			}

			//アニメーションを止める
			if (SAttackAnimInstance) {
				SAttackAnimInstance->Montage_SetPlayRate(DodgeMontage, 0.01f);
			}
		}
	}


	// 回避の移動タスクがあるなら
	if (IsValid(m_moveTask))
	{
		// タスク停止
		m_moveTask->EndTask();
		m_moveTask = nullptr;
	}
	

	LocationDelegate.Unbind();

	m_justDodgeWind = false;



	JustDodgeLocation();

	JustDodgeRotate(m_justDodgeRelativeAngle);


	////移動用の関数の引数に代入してバインド
	//LocationDelegate = FTimerDelegate::CreateUObject(this, &UGAPlayerDodge::DodgeLocation, DodgeDistance / 2);

	////回避処理開始(ほぼ毎フレーム繰り返す)
	//GetWorld()->GetTimerManager().SetTimer(DodgeTimer, LocationDelegate, 0.001 / 5, true);

	//UE_LOG(LogTemp, Warning, TEXT("【GAPlayerDodge.cpp】: PlayJustDodge"));


	//カウンター攻撃受付のタグを飛ばす
	IsCounterAttackTag = FGameplayTag::RequestGameplayTag(FName("CounterAttack"));
	if (ASC)
	{
		//タグを付与する
		ASC->AddLooseGameplayTag(IsCounterAttackTag);
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

	//カウンター攻撃のタグを破棄
	ASC->RemoveLooseGameplayTag(IsCounterAttackTag);

	// ジャスト回避攻撃のタグ削除
	ASC->RemoveLooseGameplayTag(m_justDodgeAttack);

	m_justDodgeWind = true;

	EndJustDodgeWindow();

	DodgeEnd();

	//世界をあるべき姿に
	UGameplayStatics::SetGlobalTimeDilation(World, 1);
}


void UGAPlayerDodge::DodgeLocationStart()
{
	if (!PlayerActor)return;

	//正面を取得
	FVector DodgeForward = PlayerActor->GetActorForwardVector();
	DodgeForward.Z = 0.0f;
	DodgeForward.Normalize();

	float w = (3 / 4);

	float DodgeTime2 = DodgeTime * w;

	// 移動速度
	float Speed = DodgeDistance / DodgeTime;

	// ルートモーションタスク生成
	m_moveTask =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this, NAME_None, DodgeForward, Speed, DodgeTime, false, nullptr,
			ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, 0.0f, false);
		

	// ルートモーションタスクがあるなら
	if (m_moveTask)
	{
		m_moveTask->ReadyForActivation();
	}


	////移動用の関数の引数に代入してバインド
	//LocationDelegate = FTimerDelegate::CreateUObject(this, &UGAPlayerDodge::DodgeLocation, DodgeDistance);

	////回避処理開始(ほぼ毎フレーム繰り返す)
	//GetWorld()->GetTimerManager().SetTimer(DodgeTimer, LocationDelegate, 0.001, true);
}

//回避時の移動
void UGAPlayerDodge::DodgeLocation(float Distance)
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


void UGAPlayerDodge::JustDodgeLocation()
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
		FVector DodgeLocation = PlayerActor->GetActorLocation() + (DodgeForward * JustDodgeDistance);

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

	ASC->RemoveLooseGameplayTag(IsInvincibleTag);


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

// 敵の方向を取得する
float UGAPlayerDodge::GetEnemyAngle(const AActor* EnemyActor)
{
	// プレイヤーかエネミーがないなら0を返す
	if (!PlayerActor || !EnemyActor) return 0.0f;

	// プレイヤーから敵への方向ベクトル
	FVector DirToEnemy = (EnemyActor->GetActorLocation() - PlayerActor-> GetActorLocation());
	DirToEnemy.Z = 0.0f;
	DirToEnemy.Normalize();

	// プレイヤーの正面のベクトル
	FVector PlayerForward = PlayerActor->GetActorForwardVector();
	PlayerForward.Z = 0.0f;
	PlayerForward.Normalize();

	float Dot = FVector::DotProduct(PlayerForward, DirToEnemy);
	float CrossZ = FVector::CrossProduct(PlayerForward, DirToEnemy).Z;

	float AngleInRadians = FMath::Atan2(CrossZ, Dot);
	float AngleInDegrees = FMath::RadiansToDegrees(AngleInDegrees);

	return AngleInDegrees;
}


FVector UGAPlayerDodge::GetEnemyAngleVector(const AActor* EnemyActor)
{
	FVector AngleInDegrees = FVector::ZeroVector;

	// プレイヤーかエネミーがないなら0を返す
	if (!PlayerActor || !EnemyActor) return AngleInDegrees;

	// プレイヤーから敵への方向ベクトル
	FVector DirToEnemy = (EnemyActor->GetActorLocation() - PlayerActor->GetActorLocation());
	DirToEnemy.Z = 0.0f;
	DirToEnemy.Normalize();

	AngleInDegrees = DirToEnemy;

	return AngleInDegrees;
}

// ジャスト回避自の回転
void UGAPlayerDodge::JustDodgeRotate(FVector Angle)
{
	if (UCharacterMovementComponent* MoveComp = PlayerActor->GetCharacterMovement())
	{
		FRotator rotate = Angle.Rotation();
		PlayerActor->SetActorRotation(rotate, ETeleportType::TeleportPhysics);
	}

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
