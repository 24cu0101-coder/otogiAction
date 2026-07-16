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

	UFUNCTION(BlueprintCallable)
	void SetHPPercent(float Percent);

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;
};