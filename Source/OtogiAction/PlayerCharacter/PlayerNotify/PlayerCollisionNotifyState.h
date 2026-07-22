// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayerCollisionNotifyState.generated.h"

UCLASS()
class OTOGIACTION_API UPlayerCollisionNotifyState : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	//すべてのエディタ（キャラ・武器問わず）で使い回せる引数設定
	//スフィアの半径
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float Radius = 150.f;

	//ターゲット
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	FName TargetTag = FName("Enemy");

	//ダメージ
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float Damage = 20.f;

	//スタン値の加算
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float StunPoint = 20.f;

	//ヒットストップ時間
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float HitStopDuration = 0.09f;

	//どれくらい時間を引き延ばすか？
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float HitStopTimreScale = 0.01f;

	//攻撃範囲を前後にどれだけ出すか
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float ForwardOffset = 0.01f;

	//攻撃範囲を左右にどれだけ出すか
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float SideOffset = 0.01f;

};
