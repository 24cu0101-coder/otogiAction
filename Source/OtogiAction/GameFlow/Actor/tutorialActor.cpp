
#include "TutorialActor.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "OtogiAction/UI/TutorialGuideWidget.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "OtogiAction/minions/TutorialMinionCharacter.h"


ATutorialActor::ATutorialActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // キーボード入力を受け付ける
    AutoReceiveInput = EAutoReceiveInput::Player0;
}


void ATutorialActor::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning,
        TEXT("TutorialActor BeginPlay OK"));

    APlayerController* PC =
        UGameplayStatics::GetPlayerController(this, 0);

    if (PC)
    {
        EnableInput(PC);

        if (InputComponent)
        {
            InputComponent->BindKey(
                EKeys::P,
                IE_Pressed,
                this,
                &ATutorialActor::ForceCompleteTutorial
            );

            UE_LOG(LogTemp, Warning,
                TEXT("Pキー入力を登録しました"));
        }

        // Widgetを作成して画面に表示
        if (TutorialWidgetClass)
        {
            TutorialWidget =
                CreateWidget<UTutorialGuideWidget>(
                    PC,
                    TutorialWidgetClass
                );

            if (TutorialWidget)
            {
                TutorialWidget->AddToViewport();
            }
        }
    }

    // チュートリアル開始時の画像を表示
    UpdateTutorialImage();
}


void ATutorialActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ATutorialActor::UpdateTutorialImage()
{
    if (!TutorialWidget)
    {
        return;
    }

    // 全ステップ完了
    if (CurrentStep >= TutorialImages.Num())
    {
        TutorialWidget->SetTutorialImage(
            TutorialCompleteImage);
        return;
    }

    // 現在のステップに対応する画像を表示
    if (TutorialImages.IsValidIndex(CurrentStep))
    {
        TutorialWidget->SetTutorialImage(
            TutorialImages[CurrentStep]);
    }
}

void ATutorialActor::CompleteStep(int32 StepIndex)
{
    // 現在の目的地以外は無視
    if (StepIndex != CurrentStep)
    {
        return;
    }

    // ステップクリア音
    if (TutorialClearSound)
    {
        UGameplayStatics::PlaySound2D(this, TutorialClearSound);
    }

    // 次のステップへ進む
    CurrentStep++;

    // 配列のインデックス5をクリアしたら専用の壁を解除
    if (StepIndex == 5)
    {
        if (TutorialWall5)
        {
            // 壁の当たり判定を無効化
            TArray<UPrimitiveComponent*> Components;
            TutorialWall5->GetComponents<UPrimitiveComponent>(Components);

            for (UPrimitiveComponent* Component : Components)
            {
                if (Component)
                {
                    Component->SetCollisionEnabled(
                        ECollisionEnabled::NoCollision);

                    // 壁の見た目も非表示にする
                    Component->SetVisibility(false, true);
                }
            }

            UE_LOG(LogTemp, Warning,
                TEXT("TutorialWall5を解除しました"));
        }
        else
        {
            UE_LOG(LogTemp, Error,
                TEXT("TutorialWall5が設定されていません"));
        }
    }

    // インデックス6をクリアしたら敵をすべて削除
    /*if (StepIndex == 6)
    {
        RemoveAllEnemies();

        // Playerの入力を20秒間無効にする
        APlayerController* PC = GetWorld()->GetFirstPlayerController();

        if (PC)
        {
            APawn* PlayerPawn = PC->GetPawn();

            if (PlayerPawn)
            {
                // 移動・攻撃・回避・スキルなどの入力を停止
                PlayerPawn->DisableInput(PC);

                // 20秒後に入力を再開
                GetWorld()->GetTimerManager().SetTimer(
                    PlayerControlTimerHandle,
                    [PC, PlayerPawn]()
                    {
                        if (IsValid(PC) && IsValid(PlayerPawn))
                        {
                            PlayerPawn->EnableInput(PC);
                        }
                    },
                    20.0f,
                    false
                );
            }
        }
        // チュートリアルWidgetを20秒間非表示にする
        if (TutorialWidget)
        {
            TutorialWidget->SetVisibility(ESlateVisibility::Collapsed);

            // 20秒後にチュートリアルWidgetを再表示
            GetWorld()->GetTimerManager().SetTimer(
                TutorialWidgetTimerHandle,
                [this]()
                {
                    if (TutorialWidget)
                    {
                        TutorialWidget->SetVisibility(ESlateVisibility::Visible);
                    }
                },
                20.0f,
                false
            );
        }

        // 3秒後にレベルシーケンスを再生
        GetWorld()->GetTimerManager().SetTimer(TutorialSequenceDelayTimerHandle, this, &ATutorialActor::StartTutorialSequenceAfterDelay, 3.0f, false);

    }*/


    if (StepIndex == 7)
    {
        RemoveAllEnemies();

        // インデックス7に入ってから10秒後に演出を開始
        GetWorld()->GetTimerManager().SetTimer(TutorialSequenceDelayTimerHandle, this, &ATutorialActor::StartStep7Sequence, 6.0f, false);
    }
    // すべての目的地を達成した
    if (CurrentStep >= TutorialImages.Num())
    {
        // チュートリアル完了画像を表示
        UpdateTutorialImage();

        // 既存の壁を解除
        UnlockWall();
        return;
    }

    // 次のステップの画像へ切り替える
    UpdateTutorialImage();
}

void ATutorialActor::StartStep7Sequence()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();

    // チュートリアルWidgetを非表示にする
    if (TutorialWidget)
    {
        TutorialWidget->SetVisibility(
            ESlateVisibility::Collapsed
        );

        // 15秒後にWidgetを再表示する
        GetWorld()->GetTimerManager().SetTimer(TutorialWidgetTimerHandle, [this]()
            {
                if (IsValid(this) && IsValid(TutorialWidget))
                {
                    TutorialWidget->SetVisibility(
                        ESlateVisibility::Visible
                    );
                }
            },
            12.0f,
            false
        );
    }
    // プレイヤーの入力を15秒間停止
    if (PC)
    {
        APawn* PlayerPawn = PC->GetPawn();

        if (PlayerPawn)
        {
            PlayerPawn->DisableInput(PC);

            GetWorld()->GetTimerManager().SetTimer(
                PlayerControlTimerHandle,
                [PC, PlayerPawn]()
                {
                    if (IsValid(PC) && IsValid(PlayerPawn))
                    {
                        PlayerPawn->EnableInput(PC);
                    }
                },
                12.0f,
                false
            );
        }
    }

    // レベルシーケンスを再生
    PlayTutorialClearSequence();

    // インデックス8をクリアして次へ進む
    CompleteStep(8);
}

void ATutorialActor::UnlockWall()
{
    if (!TutorialWall)
    {
        UE_LOG(LogTemp, Warning, TEXT("TutorialWallが設定されていません"));
        return;
    }

    // 壁の当たり判定を無効化
    TArray<UPrimitiveComponent*> Components;
    TutorialWall->GetComponents<UPrimitiveComponent>(Components);

    for (UPrimitiveComponent* Component : Components)
    {
        if (Component)
        {
            Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            // 壁の見た目も非表示にする
            Component->SetVisibility(false, true);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("チュートリアル終了"));
}

// レベル内の敵をすべて消す
void ATutorialActor::RemoveAllEnemies()
{
    TArray<AActor*> AllActors;

    UGameplayStatics::GetAllActorsWithTag(
        GetWorld(),
        FName("Enemy"),
        AllActors
    );

    for (AActor* Actor : AllActors)
    {
        if (!IsValid(Actor))
        {
            continue;
        }

        // それ以外の敵は従来どおり削除
        Actor->Destroy();
    }

    UE_LOG(LogTemp, Warning,
        TEXT("チュートリアル条件達成：敵の削除処理を実行しました"));
}

void ATutorialActor::PlayTutorialClearSequence()
{
    if (!TutorialClearSequence)
    {
        UE_LOG(LogTemp, Error,
            TEXT("TutorialClearSequenceが設定されていません"));
        return;
    }

    // すでに再生中なら二重再生しない
    if (TutorialSequencePlayer &&
        TutorialSequencePlayer->IsPlaying())
    {
        return;
    }

    // 演出用画像を表示
    if (TutorialCutsceneWidgetClass)
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();

        if (PC && !TutorialCutsceneWidget)
        {
            TutorialCutsceneWidget = CreateWidget<UUserWidget>(
                PC, TutorialCutsceneWidgetClass);

            if (TutorialCutsceneWidget)
            {
                TutorialCutsceneWidget->AddToViewport(100);
            }
        }
    }

    // レベルシーケンスを再生
    FMovieSceneSequencePlaybackSettings Settings;

    // ライトの演出を無限ループ
    Settings.LoopCount.Value = -1;

    // CreateLevelSequencePlayerの第4引数には通常のポインタを渡す
    ALevelSequenceActor* SequenceActor = nullptr;

    TutorialSequencePlayer =
        ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(),
            TutorialClearSequence,
            Settings,
            SequenceActor
        );

    // 作成したシーケンスアクターを保存
    TutorialSequenceActor = SequenceActor;

    if (TutorialSequencePlayer)
    {
        // 無限ループするため、終了イベントで完了画像を切り替えない
        TutorialSequencePlayer->Play();

        UE_LOG(LogTemp, Warning,
            TEXT("チュートリアル演出を無限ループで開始しました"));

        // 続いてカメラ演出を開始
        PlayCameraSequence();
    }
    else
    {
        UE_LOG(LogTemp, Error,
            TEXT("レベルシーケンスのプレイヤー作成に失敗しました"));
    }
}

void ATutorialActor::OnTutorialSequenceFinished()
{
    // 演出用画像を消す
    if (TutorialCutsceneWidget)
    {
        TutorialCutsceneWidget->RemoveFromParent();
        TutorialCutsceneWidget = nullptr;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("チュートリアル演出終了"));
}

void ATutorialActor::ForceCompleteTutorial()
{
    // 二重実行を防止
    if (CurrentStep >= 7)
    {
        return;
    }

    // 配列のインデックスを7にする
    CurrentStep = 7;
    // レベル内の敵をすべて削除
    RemoveAllEnemies();

    // 13秒後に実行するタイマーを停止
    GetWorld()->GetTimerManager().ClearTimer(
        PlayerControlTimerHandle);

    GetWorld()->GetTimerManager().ClearTimer(
        TutorialWidgetTimerHandle);

    // プレイヤーの操作を再開
    APlayerController* PC = GetWorld()->GetFirstPlayerController();

    if (PC)
    {
        PC->DisableInput(PC);
    }

    // チュートリアルUIを削除
    if (TutorialWidget)
    {
        TutorialWidget->RemoveFromParent();
        TutorialWidget = nullptr;
    }

    // 演出用UIを削除
    if (TutorialCutsceneWidget)
    {
        TutorialCutsceneWidget->RemoveFromParent();
        TutorialCutsceneWidget = nullptr;
    }

    // レベルシーケンスを停止
    if (TutorialSequencePlayer)
    {
        TutorialSequencePlayer->Stop();
    }

    // 演出用のレベルシーケンスActorを削除
    if (TutorialSequenceActor)
    {
        TutorialSequenceActor->Destroy();
        TutorialSequenceActor = nullptr;
    }

    TutorialSequencePlayer = nullptr;

    // 配列インデックス5の壁も解除
    if (TutorialWall5)
    {
        TArray<UPrimitiveComponent*> Components;
        TutorialWall5->GetComponents<UPrimitiveComponent>(Components);

        for (UPrimitiveComponent* Component : Components)
        {
            if (Component)
            {
                Component->SetCollisionEnabled(
                    ECollisionEnabled::NoCollision);

                Component->SetVisibility(false, true);
            }
        }
    }

    // 最後の壁を解除
    UnlockWall();

    UE_LOG(LogTemp, Warning,
        TEXT("Pキー：チュートリアルを強制クリアしました"));
}

void ATutorialActor::StartTutorialSequenceAfterDelay()
{
    PlayTutorialClearSequence();
}
void ATutorialActor::PlayCameraSequence()
{
    if (!CameraSequence)
    {
        UE_LOG(LogTemp, Error,
            TEXT("CameraSequenceにLS_cameraが設定されていません"));
        return;
    }

    FMovieSceneSequencePlaybackSettings Settings;

    // カメラ演出は1回だけ再生
    Settings.LoopCount.Value = 0;

    ALevelSequenceActor* SequenceActor = nullptr;

    CameraSequencePlayer =
        ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(),
            CameraSequence,
            Settings,
            SequenceActor
        );

    CameraSequenceActor = SequenceActor;

    if (CameraSequencePlayer)
    {
        CameraSequencePlayer->Play();

        UE_LOG(LogTemp, Warning,
            TEXT("LS_cameraを開始しました"));
    }
    else
    {
        UE_LOG(LogTemp, Error,
            TEXT("LS_cameraのプレイヤー作成に失敗しました"));
    }
}