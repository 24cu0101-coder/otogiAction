
#include "TutorialActor.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "OtogiAction/UI/TutorialGuideWidget.h"
#include "Blueprint/UserWidget.h"

ATutorialActor::ATutorialActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATutorialActor::BeginPlay()
{
    Super::BeginPlay();

    //Widgetを作成して画面に表示
    if (TutorialWidgetClass)
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();

        if (PC)
        {
            TutorialWidget = CreateWidget<UTutorialGuideWidget>(
                PC,
                TutorialWidgetClass
            );

            if (TutorialWidget)
            {
                TutorialWidget->AddToViewport();
            }
        }
    }

    // チュートリアル開始時の文章を表示
    UpdateTutorialText();
}

void ATutorialActor::UpdateTutorialText()
{
    if (!TutorialWidget)
    {
        return;
    }

    if (TutorialTexts.IsValidIndex(CurrentStep))
    {
        TutorialWidget->SetTutorialText(TutorialTexts[CurrentStep]);
    }
    else
    {
        TutorialWidget->SetTutorialText(
            FText::FromString(TEXT("チュートリアル完了！"))
        );
    }
}

void ATutorialActor::CompleteStep(int32 StepIndex)
{
    // 現在の目的地以外は無視
    if (StepIndex != CurrentStep)
    {
        return;
    }

    CurrentStep++;

    // すべての目的地を達成した
    if (CurrentStep >= TutorialTexts.Num())
    {
        UpdateTutorialText();
        UnlockWall();
        return;
    }

    // 次の目的地の文章へ
    UpdateTutorialText();
}

void ATutorialActor::UnlockWall()
{
    if (!TutorialWall)
    {
        UE_LOG(LogTemp, Warning, TEXT("TutorialWallが設定されていません"));
        return;
    }

    // 壁の当たり判定を無効化
    TArray<UPrimitiveComponent*> Components;
    TutorialWall->GetComponents<UPrimitiveComponent>(Components);

    for (UPrimitiveComponent* Component : Components)
    {
        if (Component)
        {
            Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            // 壁の見た目も非表示にする
            Component->SetVisibility(false, true);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("チュートリアル終了"));
}
