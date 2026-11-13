#include "MinionsHitReactionComponent.h"

#include "MinionsCharacter.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "TimerManager.h"



UMinionsHitReactionComponent::UMinionsHitReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentStance = MaxStance;
}



// Stance追加

void UMinionsHitReactionComponent::AddStance(float Value)
{
	// 大スタン中はStanceを減らさない
	if (bIsStunned)
	{

		return;
	}

	// 姿勢崩壊済みなら処理しない
	if (bStanceBroken)
	{
		return;
	}

	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetOwner());

	if (Minion && Minion->IsDead())
	{
		return;
	}


	CurrentStance -= Value;


	CurrentStance = FMath::Clamp(CurrentStance, 0.f, MaxStance);






	// 姿勢崩壊前だけ
	if (CurrentStance > 0.f)
	{
		TryPlayLightHit();
	}

}


// Stance Break

bool UMinionsHitReactionComponent::IsStanceBreak() const
{
	return CurrentStance <= 0.f;
}



// Stance Reset

void UMinionsHitReactionComponent::ResetStance()
{
	CurrentStance = MaxStance;

	bStanceBroken = false;

}


// 大きい怯み

void UMinionsHitReactionComponent::PlayHitReaction(float DamageAmount)
{
	// 攻撃キャンセル処理

	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetOwner());


	// 攻撃中は軽い怯みを発生させない
	if (Minion && Minion->bIsAttackInProgress)
	{

		return;
	}


	if (Minion && Minion->IsDead())
	{
		return;
	}

	if (Minion)
	{

		if (Minion->IsAttacking())
		{

			// 攻撃状態解除
			Minion->SetIsAttacking(false);


			// 攻撃Montage停止
			Minion->StopAnimMontage();



		}

	}



	ACharacter* Character = Cast<ACharacter>(GetOwner());


	if (!Character)
	{
		return;
	}

	float Dot = FVector::DotProduct(Character->GetActorForwardVector(), HitDirection);






	if (Dot > 0)
	{



		if (FrontHitMontage)
		{
			Character->PlayAnimMontage(FrontHitMontage);
		}

	}
	else
	{



		if (BackHitMontage)
		{
			Character->PlayAnimMontage(BackHitMontage);
		}

	}

}


// FearMax

void UMinionsHitReactionComponent::OnFearMax()
{

	Super::OnFearMax();



}



// Stun

void UMinionsHitReactionComponent::OnStunMax()
{
	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetOwner());

	if (Minion && Minion->IsDead())
	{
		return;
	}


	Super::OnStunMax();





	StopMovement();

	StopAI();



	ACharacter* Character = Cast<ACharacter>(GetOwner());


	if (Character && StunKnockdownMontage)
	{

		Character->PlayAnimMontage(StunKnockdownMontage);

	}



	GetWorld()->GetTimerManager().SetTimer(StunRecoverTimerHandle, this, &UMinionsHitReactionComponent::RecoverFromStun, StunDownDuration, false);

}



// Stun復帰

void UMinionsHitReactionComponent::RecoverFromStun()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character)
	{
		return;
	}

	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetOwner());

	if (Minion && Minion->IsDead())
	{
		return;
	}

	// 起き上がりモーションがある場合
	if (GetUpMontage)
	{
		Character->PlayAnimMontage(GetUpMontage);

		float MontageLength =
			GetUpMontage->GetPlayLength();

		GetWorld()->GetTimerManager().SetTimer(GetUpTimerHandle, this, &UMinionsHitReactionComponent::FinishRecover, MontageLength, false);
	}
	else
	{
		// モンタージュが無ければ即復帰
		FinishRecover();
	}
}

// 移動停止

void UMinionsHitReactionComponent::StopMovement()
{

	ACharacter* Character = Cast<ACharacter>(GetOwner());


	if (Character)
	{

		UCharacterMovementComponent* Move = Character->GetCharacterMovement();


		if (Move)
		{
			Move->StopMovementImmediately();
			Move->SetMovementMode(MOVE_None);
		}

	}

}



// AI停止

void UMinionsHitReactionComponent::StopAI()
{

	ACharacter* Character = Cast<ACharacter>(GetOwner());


	if (Character)
	{

		AAIController* AI = Cast<AAIController>(Character->GetController());


		if (AI)
		{

			AI->StopMovement();


			if (AI->BrainComponent)
			{
				AI->BrainComponent->StopLogic(
					TEXT("Stun"));
			}

		}

	}

}



// AI復帰

void UMinionsHitReactionComponent::RestartAI()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character)
	{
		return;
	}

	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetOwner());

	if (Minion && Minion->IsDead())
	{
		return;
	}

	if (UCharacterMovementComponent* Move =
		Character->GetCharacterMovement())
	{
		Move->SetMovementMode(MOVE_Walking);
	}

	AAIController* AI = Cast<AAIController>(Character->GetController());

	if (!AI)
	{
		return;
	}

	AI->StopMovement();

	if (AI->BrainComponent)
	{

		AI->BrainComponent->RestartLogic();
	}

}


// 攻撃方向

void UMinionsHitReactionComponent::SetHitDirection(AActor* Attacker)
{

	if (!Attacker)
	{
		return;
	}



	FVector Direction = Attacker->GetActorLocation() - GetOwner()->GetActorLocation();



	Direction.Z = 0.f;



	HitDirection = Direction.GetSafeNormal();




}

void UMinionsHitReactionComponent::TryPlayLightHit()
{
	// 大スタン中は小スタンを再生しない
	if (bIsStunned)
	{
		return;
	}

	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetOwner());

	if (Minion && Minion->IsDead())
	{
		return;
	}


	// 攻撃中は軽い怯みを発生させない
	if (Minion && Minion->bIsAttackInProgress)
	{

		return;
	}


	if (Minion)
	{

		if (Minion->IsAttacking())
		{

			Minion->SetIsAttacking(false);

			Minion->StopAnimMontage();



		}

	}



	ACharacter* Character = Cast<ACharacter>(GetOwner());


	if (!Character)
	{
		return;
	}

	//35%判定
	float RandomValue = FMath::FRand();


	if (RandomValue > LightHitChance)
	{
		return;
	}



	float Dot = FVector::DotProduct(Character->GetActorForwardVector(), HitDirection);






	//正面軽いHit

	if (Dot > 0)
	{

		if (LightFrontHitMontage)
		{
			Character->PlayAnimMontage(LightFrontHitMontage);
		}

	}


	//背面軽いHit

	else
	{

		if (LightBackHitMontage)
		{
			Character->PlayAnimMontage(LightBackHitMontage);
		}

	}

}void UMinionsHitReactionComponent::SetStanceBroken(bool Value)
{
	bStanceBroken = Value;
}


bool UMinionsHitReactionComponent::IsStanceBroken() const
{
	return bStanceBroken;
}

void UMinionsHitReactionComponent::FinishRecover()
{
	ResetStun();

	RestartAI();

}

void UMinionsHitReactionComponent::CancelStunRecovery()
{
	// 大ダウンからの復帰タイマーを停止
	GetWorld()->GetTimerManager().ClearTimer(StunRecoverTimerHandle);

	// 起き上がりモーションのタイマーを停止
	GetWorld()->GetTimerManager().ClearTimer(GetUpTimerHandle);

	// 起き上がりモーションを停止
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character && Character->GetMesh())
	{
		if (UAnimInstance* AnimInstance =
			Character->GetMesh()->GetAnimInstance())
		{
			if (GetUpMontage)
			{
				AnimInstance->Montage_Stop(0.0f, GetUpMontage);
			}
		}
	}
}

// 盾ガード入力
void UMinionsHitReactionComponent::PlayShieldGuard()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character || Character->IsPendingKillPending())
	{
		return;
	}

	bool bIsMontagePlaying = false;
	if (Character->GetMesh() && ShieldGuardMontage)
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			bIsMontagePlaying = AnimInstance->Montage_IsPlaying(ShieldGuardMontage);
		}
	}

	// 最初の入力時、またはモンタージュが中断されていた場合は盾を構える
	if (!bIsShieldGuarding || !bIsMontagePlaying)
	{
		StartShieldGuard();
	}

	if (!bIsShieldGuarding)
	{
		return;
	}

	// ガード解除タイマーをリセット（最後の入力から ShieldGuardDuration 秒後に解除）
	GetWorld()->GetTimerManager().ClearTimer(ShieldGuardEndTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		ShieldGuardEndTimerHandle,
		this,
		&UMinionsHitReactionComponent::EndShieldGuard,
		ShieldGuardDuration,
		false
	);
}

// ガード開始
void UMinionsHitReactionComponent::StartShieldGuard()
{
	//ACharacter* Character = Cast<ACharacter>(GetOwner());
	AMinionsCharacter* Minion = Cast<AMinionsCharacter>(GetOwner());

	//if (!Character || !ShieldGuardMontage)
	//{
	//	return;
	//}

	if (!Minion|| !ShieldGuardMontage)
	{
		return;
	}


	// ★追加：盾を構える瞬間に敵が攻撃中ならキャンセルする
	if (Minion->IsAttacking())
	{
		Minion->CancelAttack();
	}

	// ガード開始
	bIsShieldGuarding = true;

	// 盾を構える
	Minion->PlayAnimMontage(ShieldGuardMontage);

	// 前のタイマーを解除
	GetWorld()->GetTimerManager().ClearTimer(
		ShieldGuardHoldTimerHandle
	);

	// 0.55秒後に一時停止
	GetWorld()->GetTimerManager().SetTimer(
		ShieldGuardHoldTimerHandle,
		[this]()
		{
			ACharacter* OwnerCharacter =
				Cast<ACharacter>(GetOwner());

			if (!OwnerCharacter ||
				!bIsShieldGuarding ||
				!OwnerCharacter->GetMesh())
			{
				return;
			}

			UAnimInstance* AnimInstance =
				OwnerCharacter->GetMesh()->GetAnimInstance();

			if (AnimInstance && ShieldGuardMontage)
			{
				AnimInstance->Montage_Pause(ShieldGuardMontage);
			}
		},
		0.55f,
		false
	);
}
// ガード解除
void UMinionsHitReactionComponent::EndShieldGuard()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character)
	{
		return;
	}

	bIsShieldGuarding = false;

	// 盾を構えるタイマーを停止
	GetWorld()->GetTimerManager().ClearTimer(ShieldGuardHoldTimerHandle);

	if (Character->GetMesh())
	{
		UAnimInstance* AnimInstance =
			Character->GetMesh()->GetAnimInstance();

		if (AnimInstance && ShieldGuardMontage)
		{
			AnimInstance->Montage_Stop(
				0.2f,
				ShieldGuardMontage
			);
		}
	}
}