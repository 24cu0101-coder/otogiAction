// Fill out your copyright notice in the Description page of Project Settings.


#include "GAIaiAttack.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UGAIaiAttack::ActivateAbility(const FGameplayAbilitySpecHandle IaiAttack,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent)
{
	Super::ActivateAbility(IaiAttack, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティ取得
	ASC = GetAbilitySystemComponentFromActorInfo();

	PlayerActor = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	if (!ASC || !SheathingMontage || !IaiAttackMontage)
	{
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
				SAttackAnimInstance->Montage_SetPlayRate(IaiAttackMontage, 0.0001f);
			}
		}
	}
}


//回避開始の処理
void UGAIaiAttack::Iaistep()
{
	//世界からtimerをもらう
	GetWorld()->GetTimerManager().SetTimer(IaiTimer, this, &UGAIaiAttack::RestartIaiAttackMontage, 0.001f, true);

	FTimerHandle Timer;
	//0.2秒後回転
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UGAIaiAttack::Rotate, IaiTime, false);


}


void UGAIaiAttack::RestartIaiAttackMontage()
{

	//プレイヤーの情報と再生タスクが在れば
	if (PlayerActor)
	{
		if (UCapsuleComponent* CapsuleComp = PlayerActor->GetCapsuleComponent())
		{
			CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}

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

void UGAIaiAttack::Rotate()
{
	//タイマー停止
	GetWorld()->GetTimerManager().ClearTimer(IaiTimer);

	if (UCapsuleComponent* CapsuleComp = PlayerActor->GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}

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


	//プレイヤーの角度を取得
	FRotator IaiRotate = PlayerActor->GetActorRotation();

	//180度加算
	IaiRotate.Yaw += 180.f;

	//回転を加える
	PlayerActor->SetActorRotation(IaiRotate);



}




void UGAIaiAttack::Sheathing(FGameplayEventData Payload)
{

	if (ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		//アニメーションインスタンスを取得
		if (UAnimInstance* SheathingAnimInstance = Char->GetMesh()->GetAnimInstance())
		{
			//再生速度を0にして止める
			SheathingAnimInstance->Montage_SetPlayRate(SheathingMontage, 0.0001f);

			FTimerHandle SheathingTimer;
			//0.5秒後再生
			GetWorld()->GetTimerManager().SetTimer(SheathingTimer, this, &UGAIaiAttack::RestartMontage, 0.9f, false);
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
			UE_LOG(LogTemp, Warning, TEXT("yy"));


			//アニメーション速度を1にして再生
			SheathingAnimInstance->Montage_SetPlayRate(SheathingMontage, 1.0f);
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


