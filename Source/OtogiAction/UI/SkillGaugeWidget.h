#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillGaugeWidget.generated.h"

class UProgressBar;

UCLASS()
class OTOGIACTION_API USkillGaugeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetGaugePercent(float Percent);

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* GaugeBar;
};