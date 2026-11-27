#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPWidget.generated.h"

class UProgressBar;

UCLASS()
class OTOGIACTION_API UPlayerHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	// 毎フレーム処理
	virtual void NativeTick(const FGeometry& MyGeometry,float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetHPPercent(float Percent);

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	//HP割合
	float TargetHPPercent=1.0f;

	//現在のHP表示
	float DisplayHPPercent = 1.0f;

	//HPの減少速度
	UPROPERTY(EditAnywhere,category="HP")
	float HPDecreaseSpeed = 1.0f;
};