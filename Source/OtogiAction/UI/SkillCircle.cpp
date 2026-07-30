#include "SkillCircle.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"


void USkillCircle::UpdateSkillCircle(int32 CurrentIndex)
{
	TargetAngle = CurrentIndex * 90.0f;
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