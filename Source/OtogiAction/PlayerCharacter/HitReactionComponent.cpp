// Fill out your copyright notice in the Description page of Project Settings.


#include "HitReactionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UHitReactionComponent::UHitReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UHitReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//敵の攻撃ヒット時に呼ばれる関数
void UHitReactionComponent::PlayHitReaction(float DamageAmount)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || bIsStunned) return;

	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();

	FVector KnockbackDir = -Character->GetActorForwardVector();
	KnockbackDir.Z = 0.0f;
	KnockbackDir.Normalize();

	// 通常ヒット
	if (LightHitReactMontage)
	{
		Character->PlayAnimMontage(LightHitReactMontage);
	}
	if (MoveComp)
	{
		Character->LaunchCharacter(KnockbackDir * LightKnockbackForce, false, false);
	}
}

//スタン値がマックスになったらダウンする
void UHitReactionComponent::OnStunMax()
{
	Super::OnStunMax();


	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	//吹き飛ばしモンタージュ
	if (HeavyHitReactMontage)
	{
		Character->PlayAnimMontage(HeavyHitReactMontage);
	}

	//AddInpulse
	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->SetMovementMode(EMovementMode::MOVE_Falling);

		//吹き飛ばすベクトル：後ろ
		FVector KnockbackDir = -Character->GetActorForwardVector();
		KnockbackDir.Z = 0.0f;
		KnockbackDir.Normalize();

		//吹き飛ばすベクトル：後ろ+上
		FVector ImpulseVector = (KnockbackDir * HeavyKnockbackForce) + FVector(0.f, 0.f, 600.f);

		//吹き飛ばし
		MoveComp->AddImpulse(ImpulseVector, true);
	}


}

//起き上がり
void UHitReactionComponent::RequestGetUp()
{
	if (!bIsStunned) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (GetupMontage)
	{
		Character->PlayAnimMontage(GetupMontage);

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UHitReactionComponent::OnGetUpFinished);
		}
	}
	else
	{
		// モーションなしの場合の安全復帰
		ResetStun();
		if (Character->GetCharacterMovement())
		{
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}

//起き上がり後に移動可能へと遷移
void UHitReactionComponent::OnGetUpFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == GetupMontage)
	{
		ResetStun(); //

		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (Character && Character->GetCharacterMovement())
		{
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UHitReactionComponent::OnGetUpFinished);
		}
	}
}
