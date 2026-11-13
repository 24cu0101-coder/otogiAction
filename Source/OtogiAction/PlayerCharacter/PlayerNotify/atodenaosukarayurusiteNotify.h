// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "atodenaosukarayurusiteNotify.generated.h"


class UIaiAttackComponent;

UCLASS()
class OTOGIACTION_API UatodenaosukarayurusiteNotify : public UAnimNotify
{
	GENERATED_BODY()

	// 通知の処理
	virtual void Notify(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference)
		override;


private:

	UIaiAttackComponent* m_iaiAttackComp;




	
};
