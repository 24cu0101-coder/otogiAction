// Fill out your copyright notice in the Description page of Project Settings.


#include "BGMManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


TWeakObjectPtr<UAudioComponent>
ABGMManager::CurrentBGMComponent;

// Sets default values
ABGMManager::ABGMManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABGMManager::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoPlay)
	{
		PlayBGM();
	}
	
}

void ABGMManager::PlayBGM()
{
    if (!BGM)
    {
        UE_LOG(LogTemp, Warning, TEXT("BGMが設定されていません"));
        return;
    }

    // 以前のレベルで再生していたBGMを停止
    if (CurrentBGMComponent.IsValid())
    {
        CurrentBGMComponent->Stop();
        CurrentBGMComponent.Reset();
    }

    // 新しいBGMを再生
    BGMComponent = UGameplayStatics::SpawnSound2D(
        this,
        BGM,
        1.0f,
        1.0f,
        0.0f,
        nullptr,
        false,
        true
    );

    CurrentBGMComponent = BGMComponent;
}

void ABGMManager::StopBGM()
{
	if (BGMComponent)
	{
		BGMComponent->Stop();
	}
}

