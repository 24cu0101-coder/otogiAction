// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCollisionNotifyState.h"
#include "../../Component/Collision/AttackCollisionComponent.h"
#include "GameFramework/Character.h"

void UPlayerCollisionNotifyState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	// アニメーションを再生しているプレイヤーキャラクターを取得
	AActor* AnimOwner = MeshComp->GetOwner();
	if (!AnimOwner) return;

	// ?? 修正ポイント：アクターからコンポーネントを正しく「検索（Find）」する
	if (UAttackCollisionComponent* AttackComp = AnimOwner->FindComponentByClass<UAttackCollisionComponent>())
	{
		// ??? エディタ側で指定した引数をそのまま渡して実行！
		AttackComp->ExcuteAreaAttack(Radius, TargetTag, Damage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Notify] プレイヤーに AttackCollisionComponent が付いていません！"));
	}
}
