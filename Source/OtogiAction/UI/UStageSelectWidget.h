#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UStageSelectWidget.generated.h"

// 前方宣言：ヘッダーで必要な型だけ宣言する
class UTextBlock;
class UImage;
class UWidget;
class UTexture2D;

UCLASS()
class OTOGIACTION_API UStageSelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    // 十字キーなどでステージ選択を移動する
    // Direction = 1 なら次、-1 なら前
    UFUNCTION(BlueprintCallable, Category = "Stage Select")
    void MoveSelection(int32 Direction);

    // 選択内容を確認する
    // ※ここではレベル遷移しない。WBP側のフェード演出を利用する
    UFUNCTION(BlueprintCallable, Category = "Stage Select")
    void ConfirmSelection();

    // 現在選択中のレベル名を取得する
    // WBP側のChange Levelノードに渡すために使用する
    UFUNCTION(BlueprintPure, Category = "Stage Select")
    FName GetSelectedLevelName() const;

    // ステージ名・説明・画像・選択表示を更新する
    UFUNCTION(BlueprintCallable, Category = "Stage Select")
    void UpdateStageDisplay();

    // ステージ決定後の入力ロック
    bool bIsTransitioning = false;

protected:

    // ウィジェット生成時に呼ばれる
    virtual void NativeConstruct() override;

    // WBP内のステージ名Text Blockと紐づける
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> Stage1Text;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> Stage2Text;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> Stage3Text;

    // WBP内の説明文Text Blockと紐づける
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> StageDescriptionText;

    // WBP内のステージ画像Imageと紐づける
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UImage> StagePreviewImage;

    // 選択中のステージを強調するWidget（任意）
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UWidget> Stage1Highlight;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UWidget> Stage2Highlight;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UWidget> Stage3Highlight;

    // ステージ名：3要素を設定する
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Select")
    TArray<FText> StageNames;

    // ステージ説明：3要素を設定する
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Select")
    TArray<FText> StageDescriptions;

    // 遷移先のレベル名：3要素を設定する
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Select")
    TArray<FName> StageLevelNames;

    // ステージのプレビュー画像：3要素を設定する
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage Select")
    TArray<TObjectPtr<UTexture2D>> StagePreviewImages;

    // 選択中のステージ番号。0=1つ目、1=2つ目、2=3つ目
    UPROPERTY(BlueprintReadOnly, Category = "Stage Select")
    int32 SelectedStageIndex = 0;


};