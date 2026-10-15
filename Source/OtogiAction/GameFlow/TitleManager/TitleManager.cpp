// 担当：飯島


#include "TitleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "TimerManager.h"
#include "../Actor/BookActor.h"
#include "../Actor/ChainActor.h"

// Sets default values
ATitleManager::ATitleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATitleManager::OnButtonPressed()
{
    //鎖アクターの破壊処理を実行
    if (TargetChainActor)
    {
        TargetChainActor->BreakChain();
    }

    // ホワイトアウト開始 
    APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
    if (CameraManager)
    {
        CameraManager->StartCameraFade(0.0f, 1.0f, FadeOutDuration, FLinearColor::White, true, true);
    }

    // フェードアウト完了後に本を開いてフェードイン
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATitleManager::SwapBookAndFadeIn, FadeOutDuration);
}

void ATitleManager::SwapBookAndFadeIn()
{
    // 3. 本アクターのメッシュ切替処理を実行
    if (TargetBookActor)
    {
        TargetBookActor->OpenBook();
    }

    // 4. 白からフェードイン (From: 1.0 -> To: 0.0)
    APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
    if (CameraManager)
    {
        CameraManager->StartCameraFade(1.0f, 0.0f, FadeInDuration, FLinearColor::White, false, false);
    }

    // フェードイン完了後にレベル移動
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATitleManager::FinishSequenceAndOpenLevel, FadeInDuration);
}

void ATitleManager::FinishSequenceAndOpenLevel()
{
    if (!NextLevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(this, NextLevelName);
    }
}

