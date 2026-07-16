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
void UHitReactionComponent::PlayHitReaction(AActor* Attacker)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)return;

	//被弾アニメーション再生
	if (HitReactMontage)
	{
		Character->PlayAnimMontage(HitReactMontage);
	}

	//ノックバック
	if (Attacker && Character->GetCharacterMovement())
	{
		FVector KnockbackDir = Character->GetActorLocation() - Attacker->GetActorLocation();
		KnockbackDir.Z = 0.0f;
		KnockbackDir.Normalize();

		// 少し浮かせつつ後ろに飛ばす
		Character->LaunchCharacter(KnockbackDir * KnockbackForce + FVector(0, 0, 250.f), true, true);
	}
}
