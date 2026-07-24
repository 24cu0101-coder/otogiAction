// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyHitReactionComponent.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "BossEnemyCharacter.h"

UBossEnemyHitReactionComponent::UBossEnemyHitReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

//リアクションのモンタージュ再生
void UBossEnemyHitReactionComponent::PlayHitReaction(float DamageAmount)
{
	//すでにスタンしていたら返す
	if (bIsStunned)return;

	//通常攻撃で怯まないキャラクターならリターン
	if (bIsBoss)return;

	//このコンポーネントを所有しているアクターを取得
	AActor* OwnerActor = GetOwner();

	//敵キャラクター型にキャスト
	ABossEnemyCharacter* OwnerCharacter = Cast<ABossEnemyCharacter>(OwnerActor);

	if (OwnerCharacter)
	{
		//敵キャラクターのメッシュ取得
		USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
		if (!Mesh) return;

		if (Mesh->GetAnimInstance() && LightHitMontage)
		{
			//モンタージュ再生
			OwnerCharacter->PlayAnimMontage(LightHitMontage);
		}
	}
}

void UBossEnemyHitReactionComponent::OnStunMax()
{
	//スタン状態に
	bIsStunned = true;
	UE_LOG(LogTemp, Warning, TEXT("[%s] *** STUN MAXED OUT! ***"), *GetOwner()->GetName());

	//スタン値タイマーのリセット
	GetWorld()->GetTimerManager().ClearTimer(StunRecoveryTimerHandle);

}

//スタンのリセット関数
void UBossEnemyHitReactionComponent::RecoverFromStun()
{
	ResetStun();

}
