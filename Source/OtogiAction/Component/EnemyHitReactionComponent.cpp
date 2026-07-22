// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHitReactionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UEnemyHitReactionComponent::UEnemyHitReactionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

//エネミーのヒットリアクション
void UEnemyHitReactionComponent::PlayHitReaction(float DamageAmount)
{
	//すでにスタンしていたら返す
	if (bIsStunned)return;

	//通常攻撃で怯まないキャラクターならリターン
	if (bIsBoss)return;

	//ひるむキャラクターならひるませる
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && LightHitMontage)
	{
		Character->PlayAnimMontage(LightHitMontage);
	}
}

//スタン値がマックスになったら大ダウンする関数
void UEnemyHitReactionComponent::OnStunMax()
{
	Super::OnStunMax();

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	//大ダウンアニメーション
	if (StunKnockdownMontage)
	{
		Character->PlayAnimMontage(StunKnockdownMontage);
	}

	//経過時間で自動で起き上がるタイマー
	GetWorld()->GetTimerManager().SetTimer(StunDownTimerHandle, this, &UEnemyHitReactionComponent::RecoverFromStun, StunDownDuration, false);
}

//スタンをリセットする関数
void UEnemyHitReactionComponent::RecoverFromStun()
{
	ResetStun();
}