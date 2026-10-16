
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
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

    // レベル内の敵をすべて消す
    void RemoveAllEnemies();

protected:
    virtual void BeginPlay() override;


    // 画面に表示するWidgetのクラス
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
    TSubclassOf<UTutorialGuideWidget> TutorialWidgetClass;

    // 配列のインデックス5をクリアしたら解除する壁
    UPROPERTY(EditInstanceOnly, Category = "Tutorial")
    AActor* TutorialWall5;

    // 最後に通れるようにする壁
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Tutorial")
    TObjectPtr<AActor> TutorialWall;

    // 各チュートリアルステップの画像
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial|Images")
    TArray<TObjectPtr<UTexture2D>> TutorialImages;

    // チュートリアル完了時の画像
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial|Images")
    TObjectPtr<UTexture2D> TutorialCompleteImage;

 

private:
    UPROPERTY()
    TObjectPtr<UTutorialGuideWidget> TutorialWidget;

    int32 CurrentStep = 0;

    // 画像更新関数
    void UpdateTutorialImage();
    void UnlockWall();
};
