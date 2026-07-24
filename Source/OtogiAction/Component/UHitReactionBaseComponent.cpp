// Fill out your copyright notice in the Description page of Project Settings.


#include "UHitReactionBaseComponent.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UUHitReactionBaseComponent::UUHitReactionBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UUHitReactionBaseComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UUHitReactionBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//スタン値の加算
void UUHitReactionBaseComponent::AddStunPoint(float StunAmount)
{
	//すでにスタンしていたら返す
	if (bIsStunned)return;

	//スタン値と怯み値の加算
	CurrentStunPoint = FMath::Clamp(CurrentStunPoint + StunAmount, 0.f, MaxStunPoint);
	CurrentFearPoint = FMath::Clamp(CurrentFearPoint + StunAmount, 0.f, MaxFearPoint);

	UE_LOG(LogTemp, Log, TEXT("[%s] StunPoint Added: +%.1f (Current: %.1f / %.1f)"),
		*GetOwner()->GetName(), StunAmount, CurrentStunPoint, MaxStunPoint);

	//スタン値の自動回復タイマーのりせっとと再開始
	GetWorld()->GetTimerManager().ClearTimer(StunRecoveryTimerHandle);

	//スタン値が最大値を超えたらスタンする。超えていなければ一定時間の経過の後に減算を開始する
	if (CurrentStunPoint >= MaxStunPoint)
	{
		OnStunMax();
		return;
	}
	
	//怯み値を超えたら怯む
	if (CurrentFearPoint >= MaxFearPoint)
	{
		CurrentFearPoint = 0.f;
		OnFearMax();
	}

	//回復タイマー
	GetWorld()->GetTimerManager().SetTimer(
		StunRecoveryTimerHandle,
		this,
		&UUHitReactionBaseComponent::DecreaseStunOverTime,
		0.1f, // 0.1秒周期で減らしていく
		true, // ループ
		StunRecoveryDelay // 最初の開始ディレイ
	);

	
}

//playerとエネミー側の子クラスで実装するリアクション関数
void UUHitReactionBaseComponent::PlayHitReaction(float DamageAmount)
{

}

//子クラスで定義する怯み関数
void UUHitReactionBaseComponent::OnFearMax()
{
}

//スタン値が最大になったら
void UUHitReactionBaseComponent::OnStunMax()
{
	//スタン状態に
	bIsStunned = true;
	UE_LOG(LogTemp, Warning, TEXT("[%s] *** STUN MAXED OUT! ***"), *GetOwner()->GetName());

	//スタン値タイマーのリセット
	GetWorld()->GetTimerManager().ClearTimer(StunRecoveryTimerHandle);
}

//スタン状態の解除とタイマーのリセット
void UUHitReactionBaseComponent::ResetStun()
{
	//スタン状態の解除
	bIsStunned = false;

	//スタン値をゼロに
	CurrentStunPoint = 0.f;

	UE_LOG(LogTemp, Log, TEXT("[%s] Stun Reset"), *GetOwner()->GetName());

	//スタン値タイマーのリセット
	GetWorld()->GetTimerManager().ClearTimer(StunRecoveryTimerHandle);
}

//スタン値の減算
void UUHitReactionBaseComponent::DecreaseStunOverTime()
{
	//現在のスタン値から減算していく
	CurrentStunPoint = FMath::Clamp(CurrentStunPoint - (StunRecoveryRate * 0.1f), 0.0f, MaxStunPoint);

	UE_LOG(LogTemp, Verbose, TEXT("[%s] Stun Recovering: %.1f / %.1f"),
		*GetOwner()->GetName(), CurrentStunPoint, MaxStunPoint);

	//現在のスタン値がゼロになったらスタンタイマーをリセット
	if (CurrentStunPoint <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(StunRecoveryTimerHandle);
	}

}

