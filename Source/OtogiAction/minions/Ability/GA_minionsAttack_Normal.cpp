#include "GA_minionsAttack_Normal.h"

#include "OtogiAction/minions/MinionsCharacter.h"

#include "Engine/OverlapResult.h"

#include "AbilitySystemComponent.h"

#include "NiagaraFunctionLibrary.h"


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

	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		Minion->SetIsAttacking(true);

		UE_LOG(LogTemp, Warning,
			TEXT("Minion Attack Start"));
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

	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		Minion->SetIsAttacking(false);

		UE_LOG(LogTemp, Warning,
			TEXT("Minion Attack End"));
	}



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

	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(
			GetAvatarActorFromActorInfo());


	if (Minion)
	{
		Minion->SetIsAttacking(false);

		UE_LOG(LogTemp, Warning,
			TEXT("Minion Attack Interrupted"));
	}



	EndAbility(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		true,
		false);

}