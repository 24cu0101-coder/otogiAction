#include "MinionsHitReactionComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

UMinionsHitReactionComponent::UMinionsHitReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// 通常被弾
void UMinionsHitReactionComponent::PlayHitReaction(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("===== HIT REACTION START ====="));


	ACharacter* Character =
		Cast<ACharacter>(GetOwner());


	if (Character && LightHitMontage)
	{
		Character->PlayAnimMontage(
			LightHitMontage);
	}
}

// 怯み
void UMinionsHitReactionComponent::OnFearMax()
{
	Super::OnFearMax();

	// スタン中なら再生しない
	if (bIsStunned)
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character && LightHitMontage)
	{
		Character->PlayAnimMontage(LightHitMontage);
	}
}

// スタン
void UMinionsHitReactionComponent::OnStunMax()
{
	Super::OnStunMax();

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character && StunKnockdownMontage)
	{
		Character->PlayAnimMontage(StunKnockdownMontage);
	}

	// 一定時間後にスタン解除
	GetWorld()->GetTimerManager().SetTimer(
		StunDownTimerHandle,
		this,
		&UMinionsHitReactionComponent::RecoverFromStun,
		StunDownDuration,
		false
	);
}

// スタン解除
void UMinionsHitReactionComponent::RecoverFromStun()
{
	ResetStun();
}