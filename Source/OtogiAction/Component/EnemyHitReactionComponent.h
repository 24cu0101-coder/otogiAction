// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UHitReactionBaseComponent.h"
#include "EnemyHitReactionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UEnemyHitReactionComponent : public UUHitReactionBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyHitReactionComponent();

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
