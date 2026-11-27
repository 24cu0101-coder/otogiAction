
#include "PlayerHPWidget.h"
#include "Components/ProgressBar.h"

void UPlayerHPWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (HPBar)
    {
        HPBar->SetPercent(DisplayHPPercent);
    }
}

void UPlayerHPWidget::NativeTick(
    const FGeometry& MyGeometry,
    float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!HPBar)
    {
        return;
    }

    // 表示HPを目標HPに向かって徐々に近づける
    DisplayHPPercent = FMath::FInterpTo(
        DisplayHPPercent,
        TargetHPPercent,
        InDeltaTime,
        HPDecreaseSpeed
    );

    HPBar->SetPercent(DisplayHPPercent);
}

void UPlayerHPWidget::SetHPPercent(float Percent)
{
    TargetHPPercent = FMath::Clamp(Percent, 0.0f, 1.0f);
}
