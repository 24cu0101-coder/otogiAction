// Fill out your copyright notice in the Description page of Project Settings.


#include "MontageRoopNotify.h"
#include "AbilitySystemBlueprintLibrary.h"

void UMontageRoopNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (LoopOutTag.IsValid())
		{	
			FGameplayEventData EventData;
			EventData.Instigator = MeshComp->GetOwner();

			AActor* TargetActor = MeshComp->GetOwner();
			if (TargetActor && TargetActor->GetParentActor())
			{
				TargetActor = TargetActor->GetParentActor();
			}
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, LoopOutTag, EventData);
		}
	}
}
