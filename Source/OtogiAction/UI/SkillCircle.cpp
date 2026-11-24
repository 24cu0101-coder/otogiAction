#include "SkillCircle.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"


void USkillCircle::UpdateSkillCircle(int32 CurrentIndex)
{
	TargetAngle = CurrentIndex * 90.0f;

	// 選択中のアイコンだけ表示
	if (Skill_Kintaro)
	{
		Skill_Kintaro->SetVisibility(CurrentIndex == 0? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	}

	if (Skill_Kaguya)
	{
		Skill_Kaguya->SetVisibility(CurrentIndex == 1? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	}

	if (Skill_Urasima)
	{
		Skill_Urasima->SetVisibility(CurrentIndex == 2? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	}
}


void USkillCircle::NativeTick(
	const FGeometry& MyGeometry,
	float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);


	CurrentAngle = FMath::FInterpTo(
		CurrentAngle,
		TargetAngle,
		DeltaTime,
		12.0f
	);


	FWidgetTransform Transform;

	Transform.Angle = CurrentAngle;

	SkillRoot->SetRenderTransform(Transform);


	// アイコンを逆回転
	FWidgetTransform IconTransform;

	IconTransform.Angle = -CurrentAngle;


	if (Skill_Kintaro)
	{
		Skill_Kintaro->SetRenderTransform(IconTransform);
	}

	if (Skill_Kaguya)
	{
		Skill_Kaguya->SetRenderTransform(IconTransform);
	}

	if (Skill_Urasima)
	{
		Skill_Urasima->SetRenderTransform(IconTransform);
	}

}