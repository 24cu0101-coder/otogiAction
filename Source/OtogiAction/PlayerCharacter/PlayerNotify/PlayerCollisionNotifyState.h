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
	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float Radius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	FName TargetTag = FName("Enemy");

	UPROPERTY(EditAnywhere, Category = "Attack Settings")
	float Damage = 20.f;
};
