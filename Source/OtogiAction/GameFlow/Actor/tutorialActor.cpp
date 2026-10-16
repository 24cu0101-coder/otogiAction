
#include "TutorialActor.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
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

    // チュートリアル開始時の画像を表示
    UpdateTutorialImage();
}

void ATutorialActor::UpdateTutorialImage()
{
    if (!TutorialWidget)
    {
        return;
    }

    // 全ステップ完了
    if (CurrentStep >= TutorialImages.Num())
    {
        TutorialWidget->SetTutorialImage(
            TutorialCompleteImage);
        return;
    }

    // 現在のステップに対応する画像を表示
    if (TutorialImages.IsValidIndex(CurrentStep))
    {
        TutorialWidget->SetTutorialImage(
            TutorialImages[CurrentStep]);
    }
}


void ATutorialActor::CompleteStep(int32 StepIndex)
{
    // 現在の目的地以外は無視
    if (StepIndex != CurrentStep)
    {
        return;
    }

    // 次のステップへ進む
    CurrentStep++;

    // 配列のインデックス5をクリアしたら専用の壁を解除
    if (StepIndex == 5)
    {
        if (TutorialWall5)
        {
            // 壁の当たり判定を無効化
            TArray<UPrimitiveComponent*> Components;
            TutorialWall5->GetComponents<UPrimitiveComponent>(Components);

            for (UPrimitiveComponent* Component : Components)
            {
                if (Component)
                {
                    Component->SetCollisionEnabled(
                        ECollisionEnabled::NoCollision);

                    // 壁の見た目も非表示にする
                    Component->SetVisibility(false, true);
                }
            }

            UE_LOG(LogTemp, Warning,
                TEXT("TutorialWall5を解除しました"));
        }
        else
        {
            UE_LOG(LogTemp, Error,
                TEXT("TutorialWall5が設定されていません"));
        }
    }

    // インデックス6をクリアしたら敵をすべて削除
    if (StepIndex == 6)
    {
        RemoveAllEnemies();
    }

    // すべての目的地を達成した
    if (CurrentStep >= TutorialImages.Num())
    {
        // チュートリアル完了画像を表示
        UpdateTutorialImage();

        // 既存の壁を解除
        UnlockWall();
        return;
    }

    // 次のステップの画像へ切り替える
    UpdateTutorialImage();
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

// レベル内の敵をすべて消す
void ATutorialActor::RemoveAllEnemies()
{
    TArray<AActor*> AllActors;

    // レベル内のActorをすべて取得
    UGameplayStatics::GetAllActorsWithTag(
        GetWorld(),
        FName("Enemy"),
        AllActors
    );

    for (AActor* Actor : AllActors)
    {
        if (!Actor)
        {
            continue;
        }

        // 当たり判定を無効化
        TArray<UPrimitiveComponent*> Components;
        Actor->GetComponents<UPrimitiveComponent>(Components);

        for (UPrimitiveComponent* Component : Components)
        {
            if (Component)
            {
                Component->SetCollisionEnabled(
                    ECollisionEnabled::NoCollision
                );
            }
        }

        // 敵を非表示にする
        Actor->SetActorHiddenInGame(true);

        // 敵の処理を停止
        Actor->SetActorTickEnabled(false);

        // 敵を削除
        Actor->Destroy();
    }

    UE_LOG(LogTemp, Warning,
        TEXT("チュートリアル条件達成：敵をすべて削除しました"));
}