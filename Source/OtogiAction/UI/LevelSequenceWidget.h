#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSequenceWidget.generated.h"

class UImage;
class UTexture2D;

UCLASS()
class OTOGIACTION_API ULevelSequenceWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // 表示する画像を順番に登録
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Images")
    TArray<TObjectPtr<UTexture2D>> SequenceImages;

    // 画像ごとの表示時間（秒）
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Images")
    TArray<float> ImageDisplayTimes;

    // 画像を表示するImageウィジェット
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SequenceImage;

    float SwitchInterval = 3.0f;

private:
    int32 CurrentImageIndex = 0;

    FTimerHandle ImageSwitchTimerHandle;

    void ScheduleNextImage();
    void ShowCurrentImage();
    void SwitchNextImage();

    // フェードにかける時間（秒）
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequence|Fade",
        meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float FadeDuration = 0.5f;

    // フェード処理用タイマー
    FTimerHandle FadeTimerHandle;

    // フェード開始からの経過時間
    float FadeElapsed = 0.0f;

    // フェードアウト中かどうか
    bool bIsFadingOut = true;

    // フェード処理
    void StartFade();
    void UpdateFade();
};