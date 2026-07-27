// Fill out your copyright notice in the Description page of Project Settings.


#include "StartJumpAttackNotify.h"
#include "../BossEnemyCharacter.h"

void UStartJumpAttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp) return;

	//BossEnemyクラスのdelegate関数を実行
	ABossEnemyCharacter* EnemyCharacter = Cast<ABossEnemyCharacter>(MeshComp->GetOwner());
	EnemyCharacter->TriggerJumpAttack();
}

void UStartJumpAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
