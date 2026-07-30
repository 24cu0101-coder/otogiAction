#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCircle.generated.h"

class UCanvasPanel;
class UImage;

UCLASS()
class OTOGIACTION_API USkillCircle : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void UpdateSkillCircle(int32 CurrentIndex);

	virtual void NativeTick(
		const FGeometry& MyGeometry,
		float DeltaTime) override;


private:

	float CurrentAngle = 0.f;
	float TargetAngle = 0.f;
protected:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SkillRoot;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill_Urasima;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill_Kaguya;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill_Kintaro;

};
