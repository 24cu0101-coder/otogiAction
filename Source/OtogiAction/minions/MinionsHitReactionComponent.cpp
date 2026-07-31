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



//==============================
// Stance追加
//==============================

void UMinionsHitReactionComponent::AddStance(float Value)
{

	CurrentStance -= Value;


	CurrentStance =
		FMath::Clamp(
			CurrentStance,
			0.f,
			MaxStance);



	UE_LOG(LogTemp, Warning,
		TEXT("Current Stance : %f"),
		CurrentStance);



	// 姿勢崩壊前だけ
	if (CurrentStance > 0.f)
	{
		TryPlayLightHit();
	}

}


//==============================
// Stance Break
//==============================

bool UMinionsHitReactionComponent::IsStanceBreak() const
{
	return CurrentStance <= 0.f;
}



//==============================
// Stance Reset
//==============================

void UMinionsHitReactionComponent::ResetStance()
{
	CurrentStance = MaxStance;

	bStanceBroken = false;

	UE_LOG(LogTemp, Warning,
		TEXT("Stance Reset"));
}


//==============================
// 大きい怯み
//==============================

void UMinionsHitReactionComponent::PlayHitReaction(
	float DamageAmount)
{

	//==============================
	// 攻撃キャンセル処理
	//==============================

	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(GetOwner());


	if (Minion)
	{

		if (Minion->IsAttacking())
		{

			// 攻撃状態解除
			Minion->SetIsAttacking(false);


			// 攻撃Montage停止
			Minion->StopAnimMontage();


			UE_LOG(LogTemp, Warning,
				TEXT("MINION ATTACK CANCEL"));

		}

	}



	ACharacter* Character =
		Cast<ACharacter>(GetOwner());


	if (!Character)
	{
		return;
	}



	UE_LOG(LogTemp, Warning,
		TEXT("MINION BIG HIT"));



	float Dot =
		FVector::DotProduct(
			Character->GetActorForwardVector(),
			HitDirection);



	UE_LOG(LogTemp, Warning,
		TEXT("Hit Dot : %f"),
		Dot);



	if (Dot > 0)
	{

		UE_LOG(LogTemp, Warning,
			TEXT("FRONT HIT"));


		if (FrontHitMontage)
		{
			Character->PlayAnimMontage(
				FrontHitMontage);
		}

	}
	else
	{

		UE_LOG(LogTemp, Warning,
			TEXT("BACK HIT"));


		if (BackHitMontage)
		{
			Character->PlayAnimMontage(
				BackHitMontage);
		}

	}

}


//==============================
// FearMax
// ※今回は使わない
//==============================

void UMinionsHitReactionComponent::OnFearMax()
{

	Super::OnFearMax();


	UE_LOG(LogTemp, Warning,
		TEXT("Minion Fear Max"));

}



//==============================
// Stun
//==============================

void UMinionsHitReactionComponent::OnStunMax()
{

	Super::OnStunMax();


	UE_LOG(LogTemp, Warning,
		TEXT("MINION STUN"));



	StopMovement();

	StopAI();



	ACharacter* Character =
		Cast<ACharacter>(GetOwner());


	if (Character && StunKnockdownMontage)
	{

		Character->PlayAnimMontage(
			StunKnockdownMontage);

	}



	GetWorld()->GetTimerManager().SetTimer(
		StunRecoverTimerHandle,
		this,
		&UMinionsHitReactionComponent::RecoverFromStun,
		StunDownDuration,
		false);

}



//==============================
// Stun復帰
//==============================

void UMinionsHitReactionComponent::RecoverFromStun()
{
	ACharacter* Character =
		Cast<ACharacter>(GetOwner());

	if (!Character)
	{
		return;
	}

	// 起き上がりモーションがある場合
	if (GetUpMontage)
	{
		Character->PlayAnimMontage(GetUpMontage);

		float MontageLength =
			GetUpMontage->GetPlayLength();

		GetWorld()->GetTimerManager().SetTimer(
			GetUpTimerHandle,
			this,
			&UMinionsHitReactionComponent::FinishRecover,
			MontageLength,
			false);
	}
	else
	{
		// モンタージュが無ければ即復帰
		FinishRecover();
	}
}

//==============================
// 移動停止
//==============================

void UMinionsHitReactionComponent::StopMovement()
{

	ACharacter* Character =
		Cast<ACharacter>(GetOwner());


	if (Character)
	{

		UCharacterMovementComponent* Move =
			Character->GetCharacterMovement();


		if (Move)
		{
			Move->StopMovementImmediately();
			Move->SetMovementMode(MOVE_None);
		}

	}

}



//==============================
// AI停止
//==============================

void UMinionsHitReactionComponent::StopAI()
{

	ACharacter* Character =
		Cast<ACharacter>(GetOwner());


	if (Character)
	{

		AAIController* AI =
			Cast<AAIController>(
				Character->GetController());


		if (AI)
		{

			AI->StopMovement();


			if (AI->BrainComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("StopLogic"));
				AI->BrainComponent->StopLogic(
					TEXT("Stun"));
			}

		}

	}

}



//==============================
// AI復帰
//==============================

void UMinionsHitReactionComponent::RestartAI()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character)
	{
		return;
	}

	if (UCharacterMovementComponent* Move =
		Character->GetCharacterMovement())
	{
		Move->SetMovementMode(MOVE_Walking);
	}

	AAIController* AI =
		Cast<AAIController>(Character->GetController());

	if (!AI)
	{
		return;
	}

	AI->StopMovement();

	if (AI->BrainComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("RestartLogic"));

		AI->BrainComponent->RestartLogic();
	}

	UE_LOG(LogTemp, Warning, TEXT("RestartAI End"));
}


//==============================
// 攻撃方向
//==============================

void UMinionsHitReactionComponent::SetHitDirection(
	AActor* Attacker)
{

	if (!Attacker)
	{
		return;
	}



	FVector Direction =
		Attacker->GetActorLocation()
		-
		GetOwner()->GetActorLocation();



	Direction.Z = 0.f;



	HitDirection =
		Direction.GetSafeNormal();



	UE_LOG(LogTemp, Warning,
		TEXT("HitDirection : %s"),
		*HitDirection.ToString());

}

void UMinionsHitReactionComponent::TryPlayLightHit()
{

	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(GetOwner());


	if (Minion)
	{

		if (Minion->IsAttacking())
		{

			Minion->SetIsAttacking(false);

			Minion->StopAnimMontage();


			UE_LOG(LogTemp, Warning,
				TEXT("MINION LIGHT ATTACK CANCEL"));

		}

	}



	ACharacter* Character =
		Cast<ACharacter>(GetOwner());


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



	float Dot =
		FVector::DotProduct(
			Character->GetActorForwardVector(),
			HitDirection);



	UE_LOG(LogTemp, Warning,
		TEXT("LIGHT HIT DOT : %f"),
		Dot);



	//正面軽いHit

	if (Dot > 0)
	{

		if (LightFrontHitMontage)
		{
			Character->PlayAnimMontage(
				LightFrontHitMontage);
		}

	}


	//背面軽いHit

	else
	{

		if (LightBackHitMontage)
		{
			Character->PlayAnimMontage(
				LightBackHitMontage);
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

	UE_LOG(LogTemp, Warning,
		TEXT("STUN RECOVER"));
}