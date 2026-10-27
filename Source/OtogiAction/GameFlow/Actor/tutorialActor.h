
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "LevelSequence.h"
#include "tutorialActor.generated.h"

class UTutorialGuideWidget;
class AActor;
class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class UUserWidget;
class USoundBase;

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

    virtual void Tick(float DeltaTime) override;

public:
    //レベルシーケンス系
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial|Sequence")
    TObjectPtr<ULevelSequence> TutorialClearSequence;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial|Sequence")
    TSubclassOf<UUserWidget> TutorialCutsceneWidgetClass;

    UPROPERTY()
    TObjectPtr<ULevelSequencePlayer> TutorialSequencePlayer;

    UPROPERTY()
    TObjectPtr<ALevelSequenceActor> TutorialSequenceActor;

    UPROPERTY()
    TObjectPtr<UUserWidget> TutorialCutsceneWidget;

    UFUNCTION()
    void PlayTutorialClearSequence();

    UFUNCTION()
    void OnTutorialSequenceFinished();

    //カメラのシーケンス系
    UPROPERTY(EditAnywhere, Category = "Tutorial|Sequence")
    TObjectPtr<ULevelSequence> CameraSequence;

    UPROPERTY()
    TObjectPtr<ULevelSequencePlayer> CameraSequencePlayer;

    UPROPERTY()
    TObjectPtr<ALevelSequenceActor> CameraSequenceActor;

    void PlayCameraSequence();


    // チュートリアルを強制的にクリアする
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void ForceCompleteTutorial();

    void StartTutorialSequenceAfterDelay();

    // チュートリアルクリア時の効果音
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial|Sound")
    TObjectPtr<USoundBase> TutorialClearSound;

private:
    UPROPERTY()
    TObjectPtr<UTutorialGuideWidget> TutorialWidget;

    // チュートリアルWidgetを一時的に非表示にする
    FTimerHandle TutorialWidgetTimerHandle;

    // Playerの操作を再開するためのタイマー
    FTimerHandle PlayerControlTimerHandle;

    FTimerHandle TutorialSequenceDelayTimerHandle;

    int32 CurrentStep = 0;

    // 画像更新関数
    void UpdateTutorialImage();
    void UnlockWall();

    // 7番目のチュートリアル演出を開始
    void StartStep7Sequence();


};
