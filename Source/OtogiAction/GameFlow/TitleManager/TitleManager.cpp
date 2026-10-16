// 担当：飯島


#include "TitleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "TimerManager.h"
#include "../Actor/BookActor.h"
#include "../Actor/ChainActor.h"
#include "../../PlayerCharacter/PlayerCharacter.h"
#include "../../Component/Audio/BGMManager.h"

// Sets default values
ATitleManager::ATitleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    SoundManager = CreateDefaultSubobject<USoundManagerComponent>(TEXT("SoundManager"));

}

void ATitleManager::BeginPlay()
{
    Super::BeginPlay();

    if (SoundManager && TitleBGM)
    {
        SoundManager->PlayBGM(TitleBGM);
    }
}

void ATitleManager::OnButtonPressed()
{
    //鎖アクターの破壊処理を即座に実行
    if (TargetChainActor)
    {
        TargetChainActor->BreakChain();
    }

    //と同時に本を開く
    if (TargetBookActor)
    {
        TargetBookActor->OpenBook();
    }

    //演出を見せるため一定時間待機した後にホワイトアウトを開始
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATitleManager::StartWhiteout, HoldBeforeFadeDuration);
}

void ATitleManager::StartWhiteout()
{
    APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
    if (CameraManager)
    {
        // 白へフェードアウト (From: 0.0 -> To: 1.0)
        // bHoldWhenFinished を true にすることで白くなったまま画面を維持します
        FLinearColor WhiteColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
        CameraManager->StartCameraFade(0.0f, 1.0f, FadeOutDuration, WhiteColor, false, true);
    }

    // ホワイトアウト完了後にレベル移動
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATitleManager::FinishSequenceAndOpenLevel, FadeOutKeepDuration);
}

void ATitleManager::FinishSequenceAndOpenLevel()
{
    if (!NextLevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(this, NextLevelName);
    }
}