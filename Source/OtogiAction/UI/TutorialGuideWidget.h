
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialGuideWidget.generated.h"

class UTextBlock;

UCLASS()
class OTOGIACTION_API UTutorialGuideWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void SetTutorialText(const FText& NewText);

protected:
    // WBP_TutorialGuide内のText Blockと名前を一致させる
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TutorialText;
};
