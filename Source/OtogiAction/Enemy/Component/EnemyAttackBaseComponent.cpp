// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackBaseComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyAttackBaseComponent::UEnemyAttackBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}

//攻撃開始時に呼ばれる関数
void UEnemyAttackBaseComponent::ExecuteAttack()
{
}

//攻撃終了時に呼ばれる関数
void UEnemyAttackBaseComponent::FinishAttack(bool bSuccess)
{
}

float UEnemyAttackBaseComponent::CalculateScore(AAIController* Controller, APawn* ControlledPawn)
{
	if (!Controller || !ControlledPawn) return 0.0f;

	APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!TargetPawn) return 0.0f;

	//プレイヤーとの距離を取得
	float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), TargetPawn->GetActorLocation());
	//距離によるスコアの簡易判定
	float DistanceScore = 0.0f;

	if (Distance < MinRange)
	{
		// 最小距離より近い場合
		// 近距離攻撃なら MinRange=0 なのでここは通らない
		DistanceScore = 0.0f;
	}
	else if (Distance <= MaxRange)
	{
		// 最適距離内ならスコア 1.0
		DistanceScore = 1.0f;
	}
	else if (Distance < FadeOutRange)
	{
		// MaxRange ～ FadeOutRange の間で 1.0 から 0.0 に直線的に滑らかに落とす
		DistanceScore = FMath::GetMappedRangeValueClamped(
			FVector2D(MaxRange, FadeOutRange),
			FVector2D(1.0f, 0.0f),
			Distance
		);
	}

	//基礎優先度を掛けて最終スコア算出
	return DistanceScore * BasePriority;
}

void UEnemyAttackBaseComponent::SetEnemyState()
{
}


void UEnemyAttackBaseComponent::InitializeActionData(const FBossActionData& NewData)
{
	ActionData = NewData;
}

//指定したインデックスのモンタージュを再生する関数
bool UEnemyAttackBaseComponent::PlayAttackMontageAtIndex(int32 Index)
{
	if (!AttackMontages.IsValidIndex(Index)) return false;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return false;

	UAnimInstance* AnimInstance = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return false;

	UAnimMontage* SelectedMontage = AttackMontages[Index];
	if (SelectedMontage)
	{
		float Duration = Character->PlayAnimMontage(SelectedMontage);
		return (Duration > 0.0f);
	}

	return false;
}

// Called when the game starts
void UEnemyAttackBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyAttackBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

