#include "GA_minionsAttack_Normal.h"
#include "OtogiAction/minions/MinionsCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGA_minionsAttack_Normal::UGA_minionsAttack_Normal()
{
	InstancingPolicy =
		EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UGA_minionsAttack_Normal::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		TriggerEventData);


	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;




	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		Minion->SetIsAttacking(true);

		// 攻撃中は回転をロック
		Minion->SetAttackRotationLock(true);

		// 攻撃中は移動を止める
		Minion->GetCharacterMovement()->StopMovementImmediately();
	}

	float PlayRate = 1.0f;

	if (Minion)
	{
		PlayRate = Minion->AttackPlayRate;
	}


	//==================================================
	// 予備動作
	//==================================================

	if (PreAttackMontage)
	{


		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				PreAttackMontage,
				PlayRate);


		MontageTask->OnCompleted.AddDynamic(
			this,
			&UGA_minionsAttack_Normal::OnPreAttackCompleted);


		MontageTask->OnInterrupted.AddDynamic(
			this,
			&UGA_minionsAttack_Normal::OnPreAttackInterrupted);


		MontageTask->ReadyForActivation();

		return;
	}


	// 予備動作が設定されていない場合
	OnPreAttackCompleted();
}


//==================================================
// 予備動作終了 → 本攻撃
//==================================================

void UGA_minionsAttack_Normal::OnPreAttackCompleted()
{


	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	float PlayRate = 1.0f;

	if (Minion)
	{
		PlayRate = Minion->AttackPlayRate;
	}


	//==================================================
	// 攻撃エフェクト
	//==================================================

	if (AttackEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			AttackEffect,
			GetAvatarActorFromActorInfo()->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true);
	}


	//==================================================
	// 本攻撃
	//==================================================

	if (AttackMontage)
	{


		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				AttackMontage,
				PlayRate);


		MontageTask->OnCompleted.AddDynamic(
			this,
			&UGA_minionsAttack_Normal::OnMontageCompleted);


		MontageTask->OnInterrupted.AddDynamic(
			this,
			&UGA_minionsAttack_Normal::OnMontageInterrupted);


		MontageTask->ReadyForActivation();
	}
	else
	{


		if (Minion && !Minion->IsDead())
		{
			Minion->SetIsAttacking(false);
			Minion->SetAttackRotationLock(false);
		}


		EndAbility(
			CurrentSpecHandle,
			CurrentActorInfo,
			CurrentActivationInfo,
			true,
			false);
	}
}


//==================================================
// 予備動作中断
//==================================================

void UGA_minionsAttack_Normal::OnPreAttackInterrupted()
{

	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion && !Minion->IsDead())
	{
		Minion->SetIsAttacking(false);
		Minion->SetAttackRotationLock(false);
	}


	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);
}


//==================================================
// 本攻撃終了
//==================================================

void UGA_minionsAttack_Normal::OnMontageCompleted()
{


	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion && !Minion->IsDead())
	{
		Minion->SetIsAttacking(false);
		Minion->SetAttackRotationLock(false);
	}



	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);
}


//==================================================
// 本攻撃中断
//==================================================

void UGA_minionsAttack_Normal::OnMontageInterrupted()
{


	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		Minion->SetIsAttacking(false);
		Minion->SetAttackRotationLock(false);
	}


	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);
}

