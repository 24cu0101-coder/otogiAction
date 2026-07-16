// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAudioComponent.h"
#include "Kismet/GameplayStatics.h"



UCharacterAudioComponent::UCharacterAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}



void UCharacterAudioComponent::BeginPlay()
{
	Super::BeginPlay();

}



void UCharacterAudioComponent::PlayCharacterSound(ECharacterSoundType SoundType)
{

	if (USoundBase** FindSound = SoundList.Find(SoundType))
	{
		PlaySound(*FindSound);
	}

}



void UCharacterAudioComponent::PlaySound(USoundBase* Sound)
{

	if (Sound == nullptr)
	{
		return;
	}


	AActor* OwnerActor = GetOwner();


	if (OwnerActor == nullptr)
	{
		return;
	}



	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		Sound,
		OwnerActor->GetActorLocation()
	);

}