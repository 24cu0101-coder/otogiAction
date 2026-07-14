// Fill out your copyright notice in the Description page of Project Settings.


#include "GAStrongAttack.h"
#include "../PlayerCharacter.h"

void UGAStrongAttack::ActivateAbility(const FGameplayAbilitySpecHandle SAttack,
	const FGameplayAbilityActorInfo* playerActorInfo,
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent)
{
	Super::ActivateAbility(SAttack, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティ取得
	ASC = GetAbilitySystemComponentFromActorInfo();


	if (!ASC)
	{
		//リターン
		return;
	}		

	//アニメーション再生
	PlaySAttackMontage();

}

void UGAStrongAttack::PlaySAttackMontage()
{
	//数秒後終了処理
	FTimerHandle EndDodgTimer;
	GetWorld()->GetTimerManager().SetTimer(EndDodgTimer, this, &UGAStrongAttack::RestartSAttackMontage, 0.3f, false);


	if (SAttackMontage)
	{

		//アニメーション再生タスク
		UAbilityTask_PlayMontageAndWait* SAttackMontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy
			(this, NAME_None, SAttackMontage);

		if (SAttackMontageTask)
		{
			//SAttackMontageTask->OnCancelled.AddDynamic(this, &UGAStrongAttack::SAttackMontageEnd);
		}

		if (ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo())) {
			//アニメーションインスタンスを取得
			if (UAnimInstance* SAttackAnimInstance = Char->GetMesh()->GetAnimInstance())
			{

				SAttackMontageTask->ReadyForActivation();

				SAttackAnimInstance->Montage_SetPlayRate(SAttackMontage, 0.f);
			}
		}
	}
}


//モンタージュ終了時呼び出す
void UGAStrongAttack::SAttackMontageEnd()
{
	SAttackAbilityEnd();
}

//アビリティ終了時呼び出す
void UGAStrongAttack::SAttackAbilityEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}


void UGAStrongAttack::RestartSAttackMontage()
{
	//数秒後終了処理
	FTimerHandle EndDodgTimer;
	GetWorld()->GetTimerManager().SetTimer(EndDodgTimer, this, &UGAStrongAttack::SAttackMontageEnd, 1.f, false);

	if (ACharacter* Char = Cast<ACharacter>(GetAvatarActorFromActorInfo())) {
		//アニメーションインスタンスを取得
		if (UAnimInstance* SAttackAnimInstance = Char->GetMesh()->GetAnimInstance())
		{

			SAttackAnimInstance->Montage_SetPlayRate(SAttackMontage);
		}
	}

}