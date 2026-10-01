#include "GA_minionsAttack_Strong.h"
#include "OtogiAction/minions/MinionsCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGA_minionsAttack_Strong::UGA_minionsAttack_Strong()
{
	InstancingPolicy =
		EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UGA_minionsAttack_Strong::ActivateAbility(
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


	UE_LOG(
		LogTemp,
		Warning,
		TEXT("GA STRONG START"));


	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		// 攻撃開始
		Minion->SetIsAttacking(true);

		// 攻撃中は移動を止める
		Minion->GetCharacterMovement()->StopMovementImmediately();
	}


	// 再生速度
	float PlayRate = 1.0f;

	if (Minion)
	{
		PlayRate = Minion->AttackPlayRate;
	}


	// 予備動作Montage

	if (PreAttackMontage)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("STRONG PRE ATTACK START"));


		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				PreAttackMontage,
				PlayRate);


		MontageTask->OnCompleted.AddDynamic(
			this,
			&UGA_minionsAttack_Strong::OnPreAttackCompleted);


		MontageTask->OnInterrupted.AddDynamic(
			this,
			&UGA_minionsAttack_Strong::OnPreAttackInterrupted);


		MontageTask->ReadyForActivation();

		return;
	}


	// 予備動作が設定されていない場合
	// そのまま本攻撃へ

	OnPreAttackCompleted();
}


// 予備動作終了

void UGA_minionsAttack_Strong::OnPreAttackCompleted()
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("STRONG PRE ATTACK END"));


	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	float PlayRate = 1.0f;

	if (Minion)
	{
		PlayRate = Minion->AttackPlayRate;
	}


	// 攻撃エフェクト

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


	// 本攻撃Montage

	if (AttackMontage)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("STRONG ATTACK START"));


		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				AttackMontage,
				PlayRate);


		MontageTask->OnCompleted.AddDynamic(
			this,
			&UGA_minionsAttack_Strong::OnMontageCompleted);


		MontageTask->OnInterrupted.AddDynamic(
			this,
			&UGA_minionsAttack_Strong::OnMontageInterrupted);


		MontageTask->ReadyForActivation();
	}
	else
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("AttackMontage NULL"));


		if (Minion)
		{
			Minion->SetIsAttacking(false);
		}


		EndAbility(
			CurrentSpecHandle,
			CurrentActorInfo,
			CurrentActivationInfo,
			true,
			false);
	}
}


// 予備動作中断

void UGA_minionsAttack_Strong::OnPreAttackInterrupted()
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("STRONG PRE ATTACK INTERRUPTED"));


	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		Minion->SetIsAttacking(false);
	}


	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);
}


// 本攻撃終了

void UGA_minionsAttack_Strong::OnMontageCompleted()
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("STRONG ATTACK END"));


	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		Minion->SetIsAttacking(false);
	}


	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);
}


// 本攻撃中断

void UGA_minionsAttack_Strong::OnMontageInterrupted()
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("STRONG ATTACK INTERRUPTED"));


	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		Minion->SetIsAttacking(false);
	}


	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);
}