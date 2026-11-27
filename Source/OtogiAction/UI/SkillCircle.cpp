#include "SkillCircle.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "Engine/World.h"

void USkillCircle::NativeConstruct()
{
    Super::NativeConstruct();

    CurrentOpacity = 0.2f;
    TargetOpacity = 0.2f;

    SetRenderOpacity(CurrentOpacity);
}


void USkillCircle::UpdateSkillCircle(int32 CurrentIndex)
{
    TargetAngle = CurrentIndex * 90.0f;

    UpdateIconScale(CurrentIndex);

    CurrentOpacity = 1.0f;
    TargetOpacity = 1.0f;

    SetRenderOpacity(1.0f);

    // タイマーをリセット
    GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);

    // 3秒後に薄くする
    GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this,&USkillCircle::ResetSkillCircleOpacity,3.0f, false);
}

// 選択中のアイコンを大きくする
void USkillCircle::UpdateIconScale(int32 CurrentIndex)
{
    FVector2D NormalScale(1.0f, 1.0f);
    FVector2D SmallScale(0.5f, 0.5f);

    if (Skill_Kintaro)
    {
        Skill_Kintaro->SetRenderScale(CurrentIndex == 0 ? NormalScale : SmallScale );
    }

    if (Skill_Kaguya)
    {
        Skill_Kaguya->SetRenderScale( CurrentIndex == 1 ? NormalScale : SmallScale);
    }

    if (Skill_Urasima)
    {
        Skill_Urasima->SetRenderScale(CurrentIndex == 2 ? NormalScale : SmallScale );
    }
}


void USkillCircle::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    CurrentAngle = FMath::FInterpTo(CurrentAngle, TargetAngle, DeltaTime, 12.0f);

    FWidgetTransform Transform;
    Transform.Angle = CurrentAngle;

    SkillRoot->SetRenderTransform(Transform);

    // アイコンを逆回転＋サイズ調整
    FWidgetTransform IconTransform;

    IconTransform.Angle = -CurrentAngle;

    IconTransform.Scale = FVector2D(0.6f, 0.6f);

    if (Skill_Kintaro)
    {
        IconTransform.Scale = Skill_Kintaro->GetRenderTransform().Scale;
        IconTransform.Angle = -CurrentAngle;
        Skill_Kintaro->SetRenderTransform(IconTransform);
    }

    if (Skill_Kaguya)
    {
        IconTransform.Scale = Skill_Kaguya->GetRenderTransform().Scale;
        IconTransform.Angle = -CurrentAngle;
        Skill_Kaguya->SetRenderTransform(IconTransform);
    }

    if (Skill_Urasima)
    {
        IconTransform.Scale = Skill_Urasima->GetRenderTransform().Scale;
        IconTransform.Angle = -CurrentAngle;
        Skill_Urasima->SetRenderTransform(IconTransform);
    }

    // 不透明度を滑らかに変更
    CurrentOpacity = FMath::FInterpTo(CurrentOpacity,TargetOpacity,DeltaTime, OpacitySpeed);

    SetRenderOpacity(CurrentOpacity);
}

void USkillCircle::ResetSkillCircleOpacity()
{
    TargetOpacity = 0.3f;
}