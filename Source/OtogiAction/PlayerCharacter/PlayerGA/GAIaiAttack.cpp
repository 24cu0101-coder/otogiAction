// Fill out your copyright notice in the Description page of Project Settings.


#include "GAIaiAttack.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "../PlayerComponent/PlayerTargetComponent.h"

void UGAIaiAttack::ActivateAbility(const FGameplayAbilitySpecHandle IaiAttack,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent)
{
	Super::ActivateAbility(IaiAttack, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティ取得
	ASC = GetAbilitySystemComponentFromActorInfo();

	//プレイヤーのキャラクターをキャスト
	PlayerActor = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	OwnerCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());


	//	アビリティシステムコンポーネントとモンタージュ二つのどれか一つでもなかったら
	if (!ASC || !SheathingMontage || !IaiAttackMontage)	{

		//リターン
		return;
	}


	//納刀時のタグ
	FGameplayTag SheathingTag = FGameplayTag::RequestGameplayTag(FName("Iai.Sheathing"));

	//納刀時のイベントのタスク
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

		//UE_LOG(LogTemp, Warning, TEXT("yy"));


		SheathingEvent->ReadyForActivation();

	}



	//アニメーション再生
	PlayIaiAttackMontage();

}

void UGAIaiAttack::PlayIaiAttackMontage()
{
	//数秒後終了処理
	FTimerHandle EndDodgTimer;
	GetWorld()->GetTimerManager().SetTimer(EndDodgTimer, this, &UGAIaiAttack::Iaistep, 0.3f, false);


	if (IaiAttackMontage && SheathingMontage)
	{

		//アニメーション再生タスク
		UAbilityTask_PlayMontageAndWait* IaiMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
			(this, NAME_None, IaiAttackMontage);

		if (IaiMontageTask)
		{
			//IaiMontageTask->OnCompleted.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
			//IaiMontageTask->OnInterrupted.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
			//IaiMontageTask->OnCancelled.AddDynamic(this, &UGAIaiAttack::IaiAttackAbilityEnd);
		}


		if (ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
		{
			//アニメーションインスタンスを取得
			if (UAnimInstance* SAttackAnimInstance = Char->GetMesh()->GetAnimInstance())
			{

				IaiMontageTask->ReadyForActivation();

				//アニメーションを止める
				SAttackAnimInstance->Montage_SetPlayRate(IaiAttackMontage, 0.001f);
			}
		}
	}
}


//回避開始の処理
void UGAIaiAttack::Iaistep()
{
	//世界からtimerをもらう
	//GetWorld()->GetTimerManager().SetTimer(IaiTimer, this, &UGAIaiAttack::RestartIaiAttackMontage, 0.001f, true);

	FTimerHandle Timer;
	//0.2秒後回転
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UGAIaiAttack::IaiWarping, IaiTime, false);


}


void UGAIaiAttack::RestartIaiAttackMontage()
{

	//プレイヤーの情報と再生タスクが在れば
	if (PlayerActor)
	{	
		UE_LOG(LogTemp, Warning, TEXT("dd"));


		//プレイヤーの正面を取得
		FVector IaiForward = PlayerActor->GetActorForwardVector();

		//縦方向の動きを0に
		IaiForward.Z = 0.f;

		//ベクトル正規化
		IaiForward.Normalize();

		//最終回避距離と方向(なんか正規化)
		FVector IaiLocation = PlayerActor->GetActorLocation() + (IaiForward * IaiDistance * GetWorld()->DeltaTimeSeconds);

		//プレイヤーを移動
		PlayerActor->SetActorLocation(IaiLocation, true);


	}

}

void UGAIaiAttack::Rotate(FVector TargetLocation)
{
	//タイマー停止
	GetWorld()->GetTimerManager().ClearTimer(IaiTimer);
	//アニメーション再生タスク
	UAbilityTask_PlayMontageAndWait* SheathingMontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
		(this, NAME_None, SheathingMontage);

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

	RestartIaiAttackMontage();

	if (OwnerCharacter) {

		//コリジョンを一瞬消す
		if (UCapsuleComponent* CapsuleComp = PlayerActor->GetCapsuleComponent())
		{
			CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		}



		//相手のアクターの角度を取得
		FVector MyLoc = OwnerCharacter->GetActorLocation();
		FVector Direction = TargetLocation - MyLoc;
		Direction.Z = 0.f;
		Direction.Normalize();

		FRotator TargetRot = Direction.Rotation();

		//アクターを回転
		OwnerCharacter->SetActorRotation(TargetRot);


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

	if (ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SheathingAnimInstance = Char->GetMesh()->GetAnimInstance())
		{
			//再生速度を0にして止める
			SheathingAnimInstance->Montage_SetPlayRate(SheathingMontage, 0.001f);

			FTimerHandle SheathingTimer;
			//0.5秒後再生
			GetWorld()->GetTimerManager().SetTimer(SheathingTimer, this, &UGAIaiAttack::RestartMontage, 0.9f, false);
		}
	}
}

//専用のワーピング処理をする
void UGAIaiAttack::IaiWarping()
{
	if (ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerTargetComp = Char->FindComponentByClass<UPlayerTargetComponent>();
		if (PlayerTargetComp)
		{
			WarpTargetActor = PlayerTargetComp->GetSoftLockTarget(600.f);
			if (WarpTargetActor)
			{
				Rotate(WarpTargetActor->GetActorLocation());
			}
			else
			{
				IaiAttackMontageEnd();
			}
		}
	}
}


void UGAIaiAttack::RestartMontage()
{

	if (ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SheathingAnimInstance = Char->GetMesh()->GetAnimInstance())
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

