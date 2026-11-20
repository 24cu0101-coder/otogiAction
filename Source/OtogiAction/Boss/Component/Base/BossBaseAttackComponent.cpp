// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBaseAttackComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

// Sets default values for this component's properties
UBossBaseAttackComponent::UBossBaseAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossBaseAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



// Called every frame
void UBossBaseAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//攻撃開始関数
void UBossBaseAttackComponent::StartAttack_Implementation()
{
	bIsAttacking = true;
}

//攻撃終了関数
void UBossBaseAttackComponent::EndAttack_Implementation()
{
	bIsAttacking = false;

	//BT Taskなどの待機側に攻撃完了を通知
	OnAttackEnded.Broadcast();
}

//モンタージュ再生関数
bool UBossBaseAttackComponent::PlayAttackMontage(UAnimMontage* MontageToPlay, float PlayRate, FName StartSectionName)
{
	//モンタージュが再生されていなければ実行
	if (!MontageToPlay)
	{
		return false;
	}

	//
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			const float Duration = AnimInstance->Montage_Play(MontageToPlay, PlayRate);
			if (Duration > 0.f)
			{
				if (StartSectionName != NAME_None)
				{
					AnimInstance->Montage_JumpToSection(StartSectionName, MontageToPlay);
				}

				//モンタージュ終了時に OnMontageEndedCallback が呼ばれるようにバインドする
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &UBossBaseAttackComponent::OnMontageEndedCallback);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
				return true;
			}
		}
	}
	return false;
}

//モンタージュ終了時のコールバック
void UBossBaseAttackComponent::OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted)
{
	// 再生していた攻撃モンタージュが終わったら EndAttack を呼ぶ
	if (Montage == AttackMontage && bIsAttacking)
	{
		EndAttack();
	}
}

