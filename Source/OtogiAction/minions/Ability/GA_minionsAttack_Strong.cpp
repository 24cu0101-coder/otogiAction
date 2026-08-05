#include "GA_minionsAttack_Strong.h"

#include "OtogiAction/minions/MinionsCharacter.h"

#include "AbilitySystemComponent.h"

#include "NiagaraFunctionLibrary.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"



UGA_minionsAttack_Strong::UGA_minionsAttack_Strong()
{
	InstancingPolicy =EGameplayAbilityInstancingPolicy::InstancedPerActor;
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

	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetAvatarActorFromActorInfo());

	if (Minion)
	{
		Minion->SetIsAttacking(true);
	}

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

	// Montage再生

	float PlayRate = 1.0f;

	if (Minion)
	{
		PlayRate = Minion->AttackPlayRate;
	}

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
	}


}



// Montage終了
void UGA_minionsAttack_Strong::OnMontageCompleted()
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
void UGA_minionsAttack_Strong::OnMontageInterrupted()
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