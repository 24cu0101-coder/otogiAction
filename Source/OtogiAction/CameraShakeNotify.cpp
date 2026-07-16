// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeNotify.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

void UCameraShakeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!CameraShakeClass)return;

	//アクターを取得
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)return;

	//playerのカメラを取得
	APlayerController* PC = UGameplayStatics::GetPlayerController(OwnerActor->GetWorld(), 0);

	if (PC && PC->PlayerCameraManager)
	{
		PC->PlayerCameraManager->StartCameraShake(CameraShakeClass, ShakeScale);
	}
}
