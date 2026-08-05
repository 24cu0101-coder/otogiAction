#include "GA_minionsAttack_Middle.h"

#include "OtogiAction/minions/MinionsCharacter.h"

#include "AbilitySystemComponent.h"

#include "NiagaraFunctionLibrary.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_minionsAttack_Middle::UGA_minionsAttack_Middle()
{
	InstancingPolicy =
		EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UGA_minionsAttack_Middle::ActivateAbility(
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


	UE_LOG(LogTemp, Warning, TEXT("GA MIDDLE START"));

	//エフェクト
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


	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetAvatarActorFromActorInfo());

	float PlayRate = 1.0f;

	if (Minion)
	{
		PlayRate = Minion->AttackPlayRate;
	}

	// Montage再生
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
			&UGA_minionsAttack_Middle::OnMontageCompleted);

		MontageTask->OnInterrupted.AddDynamic(
			this,
			&UGA_minionsAttack_Middle::OnMontageInterrupted);

		MontageTask->ReadyForActivation();
	}
	else
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("AttackMontage is NULL"));
	}
}


// Montage終了
void UGA_minionsAttack_Middle::OnMontageCompleted()
{

	AMinionsCharacter* Minion =Cast<AMinionsCharacter>(GetAvatarActorFromActorInfo());


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

// Montage中断
void UGA_minionsAttack_Middle::OnMontageInterrupted()
{

	AMinionsCharacter* Minion =Cast<AMinionsCharacter>(GetAvatarActorFromActorInfo());


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