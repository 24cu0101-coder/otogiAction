
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "tutorialActor.generated.h"

class UTutorialGuideWidget;
class AActor;

UCLASS()
class OTOGIACTION_API ATutorialActor : public AActor
{
    GENERATED_BODY()

public:
    ATutorialActor();

    // 目的地に到着したときに呼び出す
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void CompleteStep(int32 StepIndex);

protected:
    virtual void BeginPlay() override;

    // チュートリアル文章
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    TArray<FText> TutorialTexts;

    // 画面に表示するWidgetのクラス
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    TSubclassOf<UTutorialGuideWidget> TutorialWidgetClass;

    // 最後に通れるようにする壁
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Tutorial")
    TObjectPtr<AActor> TutorialWall;

private:
    UPROPERTY()
    TObjectPtr<UTutorialGuideWidget> TutorialWidget;

    int32 CurrentStep = 0;

    void UpdateTutorialText();
    void UnlockWall();
};
