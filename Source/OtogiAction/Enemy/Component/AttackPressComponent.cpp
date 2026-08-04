// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackPressComponent.h"
#include "../EnemyStateSubsystem.h"
#include "GameFramework/Character.h"

UAttackPressComponent::UAttackPressComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// 最初はTickを停止しておく
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// 強攻撃は0～200のときに評価が高くなるよう設定
	MinRange = 0.f;
	MaxRange = 200.f;
	FadeOutRange = 230.f;
	BasePriority = 0.2f; // 通常移動より優先度を高く設定

}

void UAttackPressComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking) return;

	// AnimInstance が無効、あるいはモンタージュが再生終了（またはキャンセル）されたか確認
	if (!CachedAnimInstance.IsValid() || !CachedAnimInstance->Montage_IsPlaying(AttackMontage))
	{
		bIsAttacking = false;
		SetComponentTickEnabled(false); // 監視TickをOFFにする
	}
}

//攻撃開始時に呼ばれる関数
void UAttackPressComponent::ExecuteAttack()
{
	//発火
	//第一引数：半径、第二引数：対象のTag、第三引数：与ダメージ、第四引数：スタン値、第五引数：ヒットストップの時間
	//第六引数：ヒットストップ時の時間の流れの倍率、第七引数：判定をどれだけ前に出すか、第八引数：判定をどれだけ横に出すか
	StartAttackHandle.Broadcast(Radius, TargetTag, Damage, StunPoint, HitStopDuration,
		HitStopTimeScale, ForwardOffset, SideOffset);

}

//列挙型のEnemyStateをセットする関数
void UAttackPressComponent::SetEnemyState()
{
	if (UWorld* World = GetWorld())
	{
		// 2. GameInstance を取得して Subsystem を呼び出す
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UEnemyStateSubsystem* StateSubsystem = GI->GetSubsystem<UEnemyStateSubsystem>())
			{
				// 状態の変更
				StateSubsystem->CurrentState = EEnemyState::AttackPress;
			}
		}
	}

}


bool UAttackPressComponent::StartAttackPress()
{
	if (!AttackMontage) return false;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return false;

	UAnimInstance* AnimInstance = Character->GetMesh() ? Character->GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance) return false;

	float Duration = AnimInstance->Montage_Play(AttackMontage);
	if (Duration <= 0.0f)
	{
		return false;
	}

	// 状態と参照を保持してTickを開始
	CachedAnimInstance = AnimInstance;
	bIsAttacking = true;
	SetComponentTickEnabled(true);

	return true;
}
