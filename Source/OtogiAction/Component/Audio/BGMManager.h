#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "BGMManager.generated.h"

UCLASS()
class OTOGIACTION_API ABGMManager : public AActor
{
    GENERATED_BODY()

public:
    ABGMManager();

protected:
    virtual void BeginPlay() override;

public:
    // 再生するBGM
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
    TObjectPtr<USoundBase> BGM;

    // ゲーム開始時に自動再生する
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BGM")
    bool bAutoPlay = true;

    // BGMを再生
    UFUNCTION(BlueprintCallable, Category = "BGM")
    void PlayBGM();

    // BGMを停止
    UFUNCTION(BlueprintCallable, Category = "BGM")
    void StopBGM();

private:
    UPROPERTY()
    TObjectPtr<UAudioComponent> BGMComponent;

    static TWeakObjectPtr<UAudioComponent> CurrentBGMComponent;
};