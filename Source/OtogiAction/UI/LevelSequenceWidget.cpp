
#include "LevelSequenceWidget.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "TimerManager.h"
#include "Engine/World.h"

void ULevelSequenceWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 最初の画像から開始
    CurrentImageIndex = 0;

    // 画像配列またはImageウィジェットが設定されていない場合は終了
    if (SequenceImages.Num() <= 0 || !SequenceImage)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("LevelSequenceWidget: 画像またはImageウィジェットが設定されていません")
        );

        return;
    }

    // 最初の画像を表示
    ShowCurrentImage();

    // 最初の画像の表示時間を設定
    ScheduleNextImage();
}

void ULevelSequenceWidget::NativeDestruct()
{
    // ウィジェットが破棄されるときにタイマーを停止
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(
            ImageSwitchTimerHandle
        );

        GetWorld()->GetTimerManager().ClearTimer(
            FadeTimerHandle
        );
    }

 

    Super::NativeDestruct();
}

void ULevelSequenceWidget::ShowCurrentImage()
{
    if (!SequenceImage ||
        !SequenceImages.IsValidIndex(CurrentImageIndex))
    {
        return;
    }

    UTexture2D* Texture = SequenceImages[CurrentImageIndex];

    if (Texture)
    {
        SequenceImage->SetBrushFromTexture(Texture, true);
    }
    else
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("LevelSequenceWidget: %d番目の画像が未設定です"),
            CurrentImageIndex
        );
    }

    // 画像ごとの位置を設定
    if (ImagePositions.IsValidIndex(CurrentImageIndex))
    {
        SequenceImage->SetRenderTranslation(
            ImagePositions[CurrentImageIndex]
        );
    }
}
void ULevelSequenceWidget::SwitchNextImage()
{
    if (SequenceImages.Num() <= 0 || !SequenceImage)
    {
        return;
    }

    // 最後の画像なら、画像を消して停止
    if (CurrentImageIndex >= SequenceImages.Num() - 1)
    {
        SequenceImage->SetVisibility(
            ESlateVisibility::Collapsed
        );
        return;
    }

    // 次の画像へ進む
    StartFade();
}
void ULevelSequenceWidget::ScheduleNextImage()
{
    // ワールドと現在の画像インデックスを確認
    if (!GetWorld() ||
        !SequenceImages.IsValidIndex(CurrentImageIndex))
    {
        return;
    }

    // 現在の画像に設定された表示時間を取得
    // 表示時間が未設定の場合は3秒
    float DisplayTime =
        ImageDisplayTimes.IsValidIndex(CurrentImageIndex)
        ? ImageDisplayTimes[CurrentImageIndex]
        : 3.0f;

    // 表示時間は最低0.1秒
    DisplayTime = FMath::Max(DisplayTime, 0.1f);

    // 以前のタイマーを停止
    GetWorld()->GetTimerManager().ClearTimer(
        ImageSwitchTimerHandle
    );

    // 指定時間が経過したら次の画像へ切り替える
    GetWorld()->GetTimerManager().SetTimer(
        ImageSwitchTimerHandle,
        this,
        &ULevelSequenceWidget::SwitchNextImage,
        DisplayTime,
        false
    );
}

void ULevelSequenceWidget::StartFade()
{
    if (!GetWorld() || !SequenceImage)
    {
        return;
    }

    SequenceImage->SetVisibility(ESlateVisibility::Visible);

    // フェード時間が0なら即座に切り替える
    if (FadeDuration <= 0.0f)
    {
        if (CurrentImageIndex < SequenceImages.Num() - 1)
        {
            CurrentImageIndex++;
        }

        ShowCurrentImage();
        ScheduleNextImage();
        return;
    }

    // フェードアウトから開始
    bIsFadingOut = true;
    FadeElapsed = 0.0f;

    GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

    // 約60fpsでフェードを更新
    GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, this, &ULevelSequenceWidget::UpdateFade, 1.0f / 60.0f, true );
}

void ULevelSequenceWidget::UpdateFade()
{
    if (!GetWorld() || !SequenceImage)
    {
        return;
    }

    FadeElapsed += GetWorld()->GetDeltaSeconds();

    // 0～1の進行度
    float Alpha = FMath::Clamp(FadeElapsed / FadeDuration,0.0f,1.0f);

    if (bIsFadingOut)
    {
        // 現在の画像を徐々に透明にする
        SequenceImage->SetRenderOpacity(1.0f - Alpha);

        if (Alpha >= 1.0f)
        {
            // 現在の画像を完全に透明にする
            SequenceImage->SetRenderOpacity(0.0f);

            if (CurrentImageIndex < SequenceImages.Num() - 1)
            {
                CurrentImageIndex++;
            }

            ShowCurrentImage();

            // フェードインへ移行
            bIsFadingOut = false;
            FadeElapsed = 0.0f;
        }
    }
    else
    {
        // 次の画像を徐々に表示する
        SequenceImage->SetRenderOpacity(Alpha);

        if (Alpha >= 1.0f)
        {
            SequenceImage->SetRenderOpacity(1.0f);

            GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);

            // 最後の画像も、表示時間が経過したら消す
            ScheduleNextImage();
        }
    }
}