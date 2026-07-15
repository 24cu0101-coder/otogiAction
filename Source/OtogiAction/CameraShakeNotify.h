// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Camera/CameraShakeBase.h"
#include "CameraShakeNotify.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UCameraShakeNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	// ノーティファイの通知開始関数
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	

protected:

	//カメラシェイクをセットできるようにする
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase>CameraShakeClass;

	//揺れの強さ倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake")
	float ShakeScale = 1.0f;
};
