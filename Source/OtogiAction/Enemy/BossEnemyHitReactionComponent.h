// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Component/UHitReactionBaseComponent.h"
#include "BossEnemyHitReactionComponent.generated.h"

/**
 * 
 */
class ABossEnemyCharacter;

UCLASS()
class OTOGIACTION_API UBossEnemyHitReactionComponent : public UUHitReactionBaseComponent
{
	GENERATED_BODY()
	
public:
	UBossEnemyHitReactionComponent();

	//エネミー用のヒットリアクション
	virtual void  PlayHitReaction(float DamageAmount)override;

protected:
	//スタンがマックスになったときに呼ばれる
	virtual void OnStunMax() override;

	//怯み値がマックスになったら呼ばれる
	virtual void OnFearMax() override;

	//通常ヒットリアクションを受けるかどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Reaction")
	bool bIsBoss = false;

	// スタンしてダウンしている時間（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Reaction")
	float StunDownDuration = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	UAnimMontage* LightHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	UAnimMontage* StunKnockdownMontage;

private:
	FTimerHandle StunDownTimerHandle;
	void RecoverFromStun();

};
