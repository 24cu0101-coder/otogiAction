#include "GA_minionsAttack_Normal.h"


#include "Engine/OverlapResult.h"

#include "AbilitySystemComponent.h"

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


	UE_LOG(
		LogTemp,
		Warning,
		TEXT("GA NORMAL START"));



	// Montage再生

	if (AttackMontage)
	{

		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				AttackMontage);


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
		UE_LOG(
			LogTemp,
			Error,
			TEXT("AttackMontage NULL"));
	}


}



// Montage終了
void UGA_minionsAttack_Normal::OnMontageCompleted()
{

	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);

}



// Montage中断
void UGA_minionsAttack_Normal::OnMontageInterrupted()
{

	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);

}