#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialGuideWidget.generated.h"

class UImage;
class UTexture2D;

UCLASS()
class OTOGIACTION_API UTutorialGuideWidget
    : public UUserWidget
{
    GENERATED_BODY()

public:
    // チュートリアル画像を変更
    UFUNCTION(BlueprintCallable)
    void SetTutorialImage(UTexture2D* NewTexture);

protected:
    // Widget BlueprintのImageと接続
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> TutorialImage;
};