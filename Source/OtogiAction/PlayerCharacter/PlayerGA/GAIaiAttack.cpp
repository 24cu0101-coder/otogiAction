// Fill out your copyright notice in the Description page of Project Settings.


#include "GAIaiAttack.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h" 
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "../PlayerComponent/PlayerTargetComponent.h"

UGAIaiAttack::UGAIaiAttack()
{

}

void UGAIaiAttack::ActivateAbility
(const FGameplayAbilitySpecHandle IaiAttack,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent)

{
	Super::ActivateAbility(IaiAttack, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティ取得
	m_ASC = GetAbilitySystemComponentFromActorInfo();

	//プレイヤーのキャラクターをキャスト
	m_playerActor = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	m_ownerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	m_char = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	//	アビリティシステムコンポーネントとモンタージュ二つのどれか一つでもなかったら
	if (!m_ASC || !m_iaiStanceMontage || !m_iaiAttackMontage) return;
	
	// 納刀モンタージュの長さを取得
	m_montageLength = m_iaiAttackMontage->GetPlayLength()/2.5;


	// 納刀時のタグ
	FGameplayTag SheathingTag = FGameplayTag::RequestGameplayTag(FName("Iai.Sheathing"));

	// 納刀時のイベントのタスク
	UAbilityTask_WaitGameplayEvent* SheathingEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		SheathingTag,
		nullptr,
		false,
		false
	);

	//納刀時のイベントのタスクがあれば
	if (SheathingEvent)
	{
		SheathingEvent->EventReceived.AddDynamic(this, &UGAIaiAttack::Sheathing);

		SheathingEvent->ReadyForActivation();
	}

	// 納刀開始
	SheathingSword();
}

// 納刀処理
void UGAIaiAttack::SheathingSword()
{
	// 納刀モンタージュがあれば
	if (m_iaiStanceMontage)
	{
		//アニメーション再生タスク
		UAbilityTask_PlayMontageAndWait* IaiMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
			(this, NAME_None, m_iaiAttackMontage);

		if (IaiMontageTask)
		{
			//IaiMontageTask->OnCompleted.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
			//IaiMontageTask->OnInterrupted.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
			//IaiMontageTask->OnCancelled.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
		}


		if (IaiMontageTask && m_char)
		{
			//アニメーションインスタンスを取得
			if (UAnimInstance* SAttackAnimInstance = m_char->GetMesh()->GetAnimInstance())
			{

				IaiMontageTask->ReadyForActivation();

				//アニメーションを止める
				//SAttackAnimInstance->Montage_SetPlayRate(IaiAttackMontage, 0.001f);
			}
		}

		// 数秒後終了処理
		FTimerHandle EndDodgTimer;
		GetWorld()->GetTimerManager().SetTimer(EndDodgTimer, this, &UGAIaiAttack::Iaistep, m_montageLength, false);

		FGameplayTag SecondInputTag = FGameplayTag::RequestGameplayTag(FName("Iai.SecondInput"));
		UAbilityTask_WaitGameplayEvent* WiatEvetnTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,SecondInputTag,nullptr ,false,false);
		if (WiatEvetnTask)
		{
			WiatEvetnTask->EventReceived.AddDynamic(this, &UGAIaiAttack::IaiSlash);
			WiatEvetnTask->ReadyForActivation();
		}
	}
}

// 抜刀の処理
void UGAIaiAttack::IaiSlash(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("IaiSlash"));

	StartMontage();
	IaiWarping();
}



// 回避開始の処理
void UGAIaiAttack::Iaistep()
{
	//世界からtimerをもらう
	//GetWorld()->GetTimerManager().SetTimer(IaiTimer, this, &UGAIaiAttack::RestartIaiAttackMontage, 0.001f, true);

	FTimerHandle VisibleTimer;
	////0.2秒後、消える
	GetWorld()->GetTimerManager().SetTimer(VisibleTimer, this, &UGAIaiAttack::StopMontage, m_iaiTime, false);


	//FTimerHandle WarpingTimer;
	////1秒後現れる
	//GetWorld()->GetTimerManager().SetTimer(WarpingTimer, this, &UGAIaiAttack::IaiWarping, IaiTime + 0.1f, false);

}


void UGAIaiAttack::RestartIaiAttackMontage()
{

	//プレイヤーの情報と再生タスクが在れば
	if (m_playerActor)
	{


		//プレイヤーの正面を取得
		FVector IaiForward = m_playerActor->GetActorForwardVector();

		//縦方向の動きを0に
		IaiForward.Z = 0.f;

		//ベクトル正規化
		IaiForward.Normalize();

		//最終回避距離と方向(なんか正規化)
		FVector IaiLocation = m_playerActor->GetActorLocation() + (IaiForward * m_iaiDistance * GetWorld()->DeltaTimeSeconds);

		//プレイヤーを移動
		m_playerActor->SetActorLocation(IaiLocation, true);


	}

}

void UGAIaiAttack::Rotate(FVector TargetLocation)
{
	//タイマー停止
	GetWorld()->GetTimerManager().ClearTimer(m_iaiTimer);
	//アニメーション再生タスク
	UAbilityTask_PlayMontageAndWait* SheathingMontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
		(this, NAME_None, m_iaiStanceMontage);

	//納刀のアニメーションタスクがあれば
	if (SheathingMontageTask)
	{
		SheathingMontageTask->OnCompleted.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
		SheathingMontageTask->OnInterrupted.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
		SheathingMontageTask->OnCancelled.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
	}

	//どちらかのタスクが無かったら
	if (!SheathingMontageTask)
	{
		IaiAttackAbilityEnd();
	}

	//納刀アニメーション再生
	SheathingMontageTask->ReadyForActivation();


	if (m_ownerCharacter)
	{

		//コリジョンを一瞬消す
		if (UCapsuleComponent* CapsuleComp = m_playerActor->GetCapsuleComponent())
		{
			CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		}



		//相手のアクターの角度を取得
		FVector MyLoc = m_ownerCharacter->GetActorLocation();
		FVector Direction = TargetLocation - MyLoc;
		Direction.Z = 0.f;
		Direction.Normalize();

		FRotator TargetRot = Direction.Rotation();
		TargetRot.Yaw += 180.f;

		//RestartIaiAttackMontage();

		FVector IaiLocation = TargetLocation - (Direction * m_iaiDistance);
		IaiLocation.Z = MyLoc.Z;

		m_ownerCharacter->SetActorLocation(IaiLocation, false);

		//アクターを回転
		m_ownerCharacter->SetActorRotation(TargetRot);

	}

	else
	{



		////プレイヤーの角度を取得
		//FRotator IaiRotate = PlayerActor->GetActorRotation();

		////180度加算
		//IaiRotate.Yaw += 180.f;

		////回転を加える
		//PlayerActor->SetActorRotation(IaiRotate);
	}
}




void UGAIaiAttack::Sheathing(FGameplayEventData Payload)
{


	if (m_char)
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SheathingAnimInstance = m_char->GetMesh()->GetAnimInstance())
		{
			//再生速度を0にして止める
			SheathingAnimInstance->Montage_SetPlayRate(m_iaiStanceMontage, 0.001f);



			FTimerHandle SheathingTimer;
			//0.5秒後再生
			GetWorld()->GetTimerManager().SetTimer(SheathingTimer, this, &UGAIaiAttack::RestartMontage, 0.9f, false);
		}
	}
}

//専用のワーピング処理をする
void UGAIaiAttack::IaiWarping()
{
	PlayerVisible(true);

	if (m_char)
	{
		m_playerTargetComp = m_char->FindComponentByClass<UPlayerTargetComponent>();
		if (m_playerTargetComp)
		{
			m_warpTargetActor = m_playerTargetComp->GetSoftLockTarget(600.f);
			if (m_warpTargetActor)
			{
				Rotate(m_warpTargetActor->GetActorLocation());
			}
			else
			{
				IaiAttackMontageEnd();
			}
		}
	}
}

//プレイヤーの姿を切り替える
void UGAIaiAttack::PlayerVisible(bool Visible)
{
	if (m_char)
	{
		if (USkeletalMeshComponent* Mesh = m_char->GetMesh())
		{
			Mesh->SetVisibility(Visible, true);
		}
	}
}

//姿を消す関数
void UGAIaiAttack::IaiVisible()
{

	//PlayerVisible(false);
}

void UGAIaiAttack::StopMontage()
{
	if (m_char)
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SAttackAnimInstance = m_char->GetMesh()->GetAnimInstance())
		{

			//IaiMontageTask->ReadyForActivation();

			//アニメーションを止める
			SAttackAnimInstance->Montage_SetPlayRate(m_iaiAttackMontage, 0.001f);
		}
	}
}

void UGAIaiAttack::StartMontage()
{
	if (m_char)
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SAttackAnimInstance = m_char->GetMesh()->GetAnimInstance())
		{

			//IaiMontageTask->ReadyForActivation();

			//アニメーションを止める
			SAttackAnimInstance->Montage_SetPlayRate(m_iaiAttackMontage, 1.0f);
		}
	}

}

void UGAIaiAttack::RestartMontage()
{
	if (m_char)
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SheathingAnimInstance = m_char->GetMesh()->GetAnimInstance())
		{
			IaiAttackMontageEnd();
		}
	}
}




//モンタージュ終了時呼び出す
void UGAIaiAttack::IaiAttackMontageEnd()
{
	IaiAttackAbilityEnd();
}

//アビリティ終了時呼び出す
void UGAIaiAttack::IaiAttackAbilityEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

