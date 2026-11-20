// Fill out your copyright notice in the Description page of Project Settings.


#include "BossComboAttackComponent.h"

//攻撃開始関数
void UBossComboAttackComponent::StartAttack_Implementation()
{
	// 既に攻撃中なら処理を中断
	if (bIsAttacking)
	{
		return;
	}

	// 親クラスの攻撃開始処理（bIsAttacking = true など）を実行
	Super::StartAttack_Implementation();

	// 子クラス側でモンタージュを再生する
	// （親のヘルパー関数を使う場合）
	if (PlayAttackMontage(AttackMontage))
	{
		// モンタージュ再生に成功したときの近接攻撃固有処理
		UE_LOG(LogTemp, Log, TEXT("Melee Montage Played!"));
	}
	else
	{
		// 再生に失敗した場合は攻撃状態をリセット
		EndAttack();
	}
}

//攻撃終了関数
void UBossComboAttackComponent::EndAttack_Implementation()
{
	// ここに近接攻撃特有の終了処理を書く（例：武器のコリジョンを無効化）
	UE_LOG(LogTemp, Log, TEXT("Melee Attack Ended!"));

	// 親クラスの処理を実行
	Super::EndAttack_Implementation();
}
