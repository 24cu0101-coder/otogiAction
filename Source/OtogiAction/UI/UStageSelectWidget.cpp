#include "UStageSelectWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Widget.h"
#include "Engine/Texture2D.h"

void UStageSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ステージ情報が3つずつ設定されているか確認する
    if (StageNames.Num() != 3 ||
        StageDescriptions.Num() != 3 ||
        StageLevelNames.Num() != 3 ||
        StagePreviewImages.Num() != 3)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("StageSelectWidget: ステージ情報を3つ設定してください")
        );
    }

    // 画面を開いたときは最初のステージを選択する
    SelectedStageIndex = 0;

    // 初期表示を更新する
    UpdateStageDisplay();
}

void UStageSelectWidget::MoveSelection(int32 Direction)
{
    // ステージ情報が3つ揃っていなければ移動しない
    if (StageNames.Num() != 3)
    {
        return;
    }

    // 選択番号を0～2の範囲で循環させる
    // 例：0から左へ移動すると2になる
    SelectedStageIndex =
        (SelectedStageIndex + Direction % 3 + 3) % 3;

    // 選択が変わったので画面を更新する
    UpdateStageDisplay();
}

void UStageSelectWidget::ConfirmSelection()
{
    // 現在の選択番号に対応するレベルが存在するか確認する
    if (!StageLevelNames.IsValidIndex(SelectedStageIndex) ||
        StageLevelNames[SelectedStageIndex].IsNone())
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("StageSelectWidget: 遷移先レベルが設定されていません")
        );

        return;
    }

    // ここではレベル遷移しない
    // WBP側でフェードを実行したあと、
    // GetSelectedLevelName() の戻り値を Change Level に渡す
}

FName UStageSelectWidget::GetSelectedLevelName() const
{
    // 選択番号が配列の範囲外なら空の名前を返す
    if (!StageLevelNames.IsValidIndex(SelectedStageIndex))
    {
        return NAME_None;
    }

    // 選択中のステージに対応するレベル名を返す
    return StageLevelNames[SelectedStageIndex];
}

void UStageSelectWidget::UpdateStageDisplay()
{
    // 選択中のステージ名を表示する
    if (Stage1Text)
    {
        Stage1Text->SetText(
            StageNames.IsValidIndex(0)
            ? StageNames[0]
            : FText::FromString(TEXT("ステージ1"))
        );
    }

    if (Stage2Text)
    {
        Stage2Text->SetText(
            StageNames.IsValidIndex(1)
            ? StageNames[1]
            : FText::FromString(TEXT("ステージ2"))
        );
    }

    if (Stage3Text)
    {
        Stage3Text->SetText(
            StageNames.IsValidIndex(2)
            ? StageNames[2]
            : FText::FromString(TEXT("ステージ3"))
        );
    }

    // 選択中のステージ説明を表示する
    if (StageDescriptions.IsValidIndex(SelectedStageIndex) &&
        StageDescriptionText)
    {
        StageDescriptionText->SetText(
            StageDescriptions[SelectedStageIndex]
        );
    }

    // 選択中のステージ画像を表示する
    if (StagePreviewImages.IsValidIndex(SelectedStageIndex) &&
        StagePreviewImage)
    {
        StagePreviewImage->SetBrushFromTexture(
            StagePreviewImages[SelectedStageIndex]
        );
    }

    // 1つ目のステージが選択中なら強調する
    if (Stage1Highlight)
    {
        Stage1Highlight->SetVisibility(
            SelectedStageIndex == 0
            ? ESlateVisibility::Visible
            : ESlateVisibility::Hidden
        );
    }

    // 2つ目のステージが選択中なら強調する
    if (Stage2Highlight)
    {
        Stage2Highlight->SetVisibility(
            SelectedStageIndex == 1
            ? ESlateVisibility::Visible
            : ESlateVisibility::Hidden
        );
    }

    // 3つ目のステージが選択中なら強調する
    if (Stage3Highlight)
    {
        Stage3Highlight->SetVisibility(
            SelectedStageIndex == 2
            ? ESlateVisibility::Visible
            : ESlateVisibility::Hidden
        );
    }
}