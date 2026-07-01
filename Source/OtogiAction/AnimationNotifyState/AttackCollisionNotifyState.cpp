// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCollisionNotifyState.h"

void UAttackCollisionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner()) return;

	//アニメーションを再生しているアクターから、コリジョンコンポーネントを探す
	UAttackCollisionComponent* CollisionComp = MeshComp->GetOwner()->FindComponentByClass<UAttackCollisionComponent>();
	if (CollisionComp)
	{
		//コリジョン生成
		CollisionComp->AttackCollisionWindow(CollisionType, Damage);
	}
}

void UAttackCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetOwner()) return;

	UAttackCollisionComponent* CollisionComp = MeshComp->GetOwner()->FindComponentByClass<UAttackCollisionComponent>();
	if (CollisionComp)
	{
		// 判定をOFFにする
		CollisionComp->AttackCollisionCloseWindow(CollisionType);
	}
}
