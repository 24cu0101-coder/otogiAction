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
	if (!Character || bIsDowned) return;

	UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();

	// キャラクターの真後ろ方向を計算
	FVector KnockbackDir = -Character->GetActorForwardVector();
	KnockbackDir.Z = 0.0f;
	KnockbackDir.Normalize();

	if (DamageAmount < HeavyDamageThreshold)
	{
		// 1. 小ダメージ：のけぞり
		if (LightHitReactMontage)
		{
			Character->PlayAnimMontage(LightHitReactMontage);
		}
		if (MoveComp)
		{
			// 地面や空中に関わらず、後ろに少し弾く
			Character->LaunchCharacter(KnockbackDir * LightKnockbackForce, true, true);
		}
	}
	else
	{
		// 2. 大ダメージ：吹っ飛びダウン
		bIsDowned = true;

		if (MoveComp)
		{
			// ダウン中は勝手に動けないように移動モードを一時無効化
			MoveComp->DisableMovement();

			// 斜め上後ろ方向に大きく吹き飛ばす
			FVector LaunchVelocity = (KnockbackDir * HeavyKnockbackForce) + FVector(0.f, 0.f, 450.f);
			Character->LaunchCharacter(LaunchVelocity, true, true);
		}

		if (HeavyHitReactMontage)
		{
			Character->PlayAnimMontage(HeavyHitReactMontage);
		}
	}
}

//起き上がり
void UHitReactionComponent::RequestGetUp()
{
	if (!bIsDowned) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (GetupMontage)
	{
		// 起き上がりモーションを再生
		Character->PlayAnimMontage(GetupMontage);

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// アニメーション終了イベントをバインド
			AnimInstance->OnMontageEnded.AddDynamic(this, &UHitReactionComponent::OnGetUpFinished);
		}
	}
	else
	{
		// モーションが設定されていない場合の安全策（即座に復帰）
		bIsDowned = false;
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
		bIsDowned = false;

		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (Character && Character->GetCharacterMovement())
		{
			// 再び自由に歩き回れるように移動モードを戻す
			Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}

		// イベントのバインドを解除してメモリリークを防ぐ
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UHitReactionComponent::OnGetUpFinished);
		}
	}
}
