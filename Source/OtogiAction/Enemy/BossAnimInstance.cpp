// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "BossCharacter.h"
#include "BossStrafeComponent.h"
#include "KismetAnimationLibrary.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BossCharacter = Cast<ABossCharacter>(TryGetPawnOwner());
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!BossCharacter)
	{
		BossCharacter = Cast<ABossCharacter>(TryGetPawnOwner());
	}
	if (!BossCharacter) return;

	// 1. ワールド空間での速度を取得（Z軸の上下移動は除外）
	FVector Velocity = BossCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	GroundSpeed = Velocity.Size();

	// 2. キャラクターの向きを基準とした移動角度（-180 ～ 180度）を計算
	const FRotator ActorRotation = BossCharacter->GetActorRotation();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, ActorRotation);

	// 3. ローカル座標系（前方向・右方向）の速度成分に分解（内積を利用）
	ForwardSpeed = FVector::DotProduct(Velocity, BossCharacter->GetActorForwardVector());
	RightSpeed = FVector::DotProduct(Velocity, BossCharacter->GetActorRightVector());

	// 4. StrafeComponentから現在の見合い状態を取得
	if (UBossStrafeComponent* StrafeComp = BossCharacter->FindComponentByClass<UBossStrafeComponent>())
	{
		bIsStrafing = StrafeComp->GetStrafeFlg();
	}
}
