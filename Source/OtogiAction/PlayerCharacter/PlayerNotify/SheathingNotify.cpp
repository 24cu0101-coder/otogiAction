// Fill out your copyright notice in the Description page of Project Settings.


#include "SheathingNotify.h"
#include "AbilitySystemBlueprintLibrary.h"

void USheathingNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{		
		if (SheathingTag.IsValid())
		{

			UE_LOG(LogTemp, Warning, TEXT("Notify"),
				*SheathingTag.ToString(),
				*MeshComp->GetOwner()->GetName());

			FGameplayEventData EventData;
			EventData.Instigator = MeshComp->GetOwner();

			AActor* TargetActor = MeshComp->GetOwner();
			if (TargetActor && TargetActor->GetParentActor())
			{
				TargetActor = TargetActor->GetParentActor();
			}
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, SheathingTag, EventData);
		}
	}
}
