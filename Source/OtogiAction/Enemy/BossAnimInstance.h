// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	// 現在の移動スピード（全体の速さ）
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0.0f;

	// 進行方向の角度（-180.0f ～ 180.0f：0=前, 90=右, -90=左, ±180=後ろ）
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction = 0.0f;

	// 前後方向のローカル速度（BlendSpaceでX/Y速度を使う場合用）
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float ForwardSpeed = 0.0f;

	// 左右方向のローカル速度（左移動ならマイナス、右移動ならプラス）
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float RightSpeed = 0.0f;

	// 見合い中かどうかのフラグ（AnimBP側でステート切り替えに使う場合）
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsStrafing = false;

private:
	UPROPERTY()
	TObjectPtr<class ABossCharacter> BossCharacter;
};
