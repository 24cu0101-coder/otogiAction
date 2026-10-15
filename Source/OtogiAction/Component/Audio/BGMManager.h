//担当：イイジマ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "BGMManager.generated.h"

UCLASS()
class OTOGIACTION_API USoundManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    //コンストラクタ
    USoundManagerComponent();

protected:

    //始まったら処理される
    virtual void BeginPlay() override;

public:
     // BGM再生関数
    UFUNCTION(BlueprintCallable, Category = "Sound|BGM")
    void PlayBGM(USoundBase* Sound);

    
    //BGM停止関数
    UFUNCTION(BlueprintCallable, Category = "Sound|BGM")
    void StopBGM();

    //BGM再開関数
    UFUNCTION(BlueprintCallable, Category = "Sound|BGM")
    void ResumeBGM();


    //==================================================
    // Loop Sound
    //==================================================

    //ループサウンド再生関数
    UFUNCTION(BlueprintCallable, Category = "Sound|Loop")
    UAudioComponent* PlayLoopSound(USoundBase* Sound);

    //LoopSound停止関数
    UFUNCTION(BlueprintCallable, Category = "Sound|Loop")
    void StopLoopSound(UAudioComponent* AudioComponent);

    //LoopSound再開関数
    UFUNCTION(BlueprintCallable, Category = "Sound|Loop")
    void ResumeLoopSound(UAudioComponent* AudioComponent);


    //==================================================
    // Sound
    //==================================================

    //サウンド再生関数
    UFUNCTION(BlueprintCallable, Category = "Sound")
    void PlaySound(USoundBase* Sound);


    //==================================================
    // Animation Notify
    //==================================================

    //AnimNotify用サウンド再生関数
    UFUNCTION(BlueprintCallable, Category = "Sound|Animation")
    void PlayAnimNotifySound(USoundBase* Sound);


private:

    //==================================================
    // BGM
    //==================================================
    //現在再生しているBGM
    UPROPERTY()
    TObjectPtr<UAudioComponent> m_BGMComponent;


    //==================================================
    // Loop Sound
    //==================================================

    //現在再生中のLoopSound
    UPROPERTY()
    TArray<TObjectPtr<UAudioComponent>> m_LoopSoundComponents;
};