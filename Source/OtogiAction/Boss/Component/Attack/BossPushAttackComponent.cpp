// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPushAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBossPushAttackComponent::UBossPushAttackComponent()
{
	// ベースのコンストラクタ設定を上書きし、ジャンプ中だけTickを回すように最適化
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBossPushAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsJumping || !CachedOwnerCharacter)
	{
		return;
	}

	ElapsedTime += DeltaTime;
	const float Alpha = FMath::Clamp(ElapsedTime / JumpDuration, 0.0f, 1.0f);

	// 水平移動の補間
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

	// サイン波による放物線（山の高さ）を加算
	NewLocation.Z += FMath::Sin(Alpha * PI) * JumpArcHeight;

	// Sweep=true で壁や床へのめり込みを防止
	CachedOwnerCharacter->SetActorLocation(NewLocation, true);

	if (Alpha >= 1.0f)
	{
		EndJumpMovement();
	}
}

//攻撃処理の開始関数
void UBossPushAttackComponent::StartAttack_Implementation()
{
	// 親クラスの処理でbIsAttackingをtrueに
	Super::StartAttack_Implementation();

	CachedOwnerCharacter = Cast<ACharacter>(GetOwner());

	// 親クラスに用意されている AttackMontage を再生
	if (!PlayAttackMontage(AttackMontage))
	{
		// モンタージュ再生に失敗した場合は即座に攻撃終了を通知してBTがスタックするのを防ぐ
		EndAttack();
	}
}

//攻撃処理の終了関数
void UBossPushAttackComponent::EndAttack_Implementation()
{
	// 攻撃が中断された場合などに備え、ジャンプ移動中なら強制終了する
	if (bIsJumping)
	{
		EndJumpMovement();
	}

	// 親クラスの処理（bIsAttacking = false と OnAttackEnded.Broadcast()）
	Super::EndAttack_Implementation();
}

//飛びつき開始
void UBossPushAttackComponent::BeginJumpMovement(AActor* TargetActor, float Duration, float ArcHeight)
{
	if (!bIsAttacking || !TargetActor || Duration <= 0.0f)
	{
		return;
	}

	if (!CachedOwnerCharacter)
	{
		CachedOwnerCharacter = Cast<ACharacter>(GetOwner());
	}
	if (!CachedOwnerCharacter)
	{
		return;
	}

	StartLocation = CachedOwnerCharacter->GetActorLocation();
	TargetLocation = TargetActor->GetActorLocation();

	// ターゲットの少し手前に着地するようにオフセット計算
	const FVector ToTarget2D = (TargetLocation - StartLocation).GetSafeNormal2D();
	if (!ToTarget2D.IsNearlyZero())
	{
		TargetLocation -= ToTarget2D * LandingOffsetDistance;

		// ターゲットの方向へボスの向きを合わせる
		FRotator LookAtRot = ToTarget2D.Rotation();
		CachedOwnerCharacter->SetActorRotation(LookAtRot);
	}

	JumpDuration = Duration;
	JumpArcHeight = ArcHeight;
	ElapsedTime = 0.0f;
	bIsJumping = true;

	// 重力の影響を切って放物線移動を制御する
	if (UCharacterMovementComponent* MoveComp = CachedOwnerCharacter->GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->SetMovementMode(MOVE_Flying);
	}

	SetComponentTickEnabled(true);
}

//飛びつき終了
void UBossPushAttackComponent::EndJumpMovement()
{
	if (!bIsJumping)
	{
		return;
	}

	bIsJumping = false;
	SetComponentTickEnabled(false);

	if (CachedOwnerCharacter)
	{
		if (UCharacterMovementComponent* MoveComp = CachedOwnerCharacter->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_Walking);
		}
	}
}
