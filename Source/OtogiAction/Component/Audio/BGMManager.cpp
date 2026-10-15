//担当：イイジマ

#include "BGMManager.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

//コンストラクタ
USoundManagerComponent::USoundManagerComponent()
{
	
}

//生成されたら処理される
void USoundManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

//BGM再生関数
void USoundManagerComponent::PlayBGM(USoundBase* Sound)
{
    if (!Sound)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("SoundManagerComponent::PlayBGM - Soundが設定されていません")
        );

        return;
    }

    // 現在のBGMを停止
    if (m_BGMComponent)
    {
        m_BGMComponent->Stop();
        m_BGMComponent = nullptr;
    }

    // 新しいBGMを再生
    m_BGMComponent = UGameplayStatics::SpawnSound2D(
        this,
        Sound,
        1.0f,  // 音量
        1.0f,  // 音の高さ
        0.0f   // スタートタイム
    );
}

//BGMの停止
void USoundManagerComponent::StopBGM()
{
    if (!m_BGMComponent)
    {
        return;
    }

    m_BGMComponent->Stop();
}

//BGMの再開
void USoundManagerComponent::ResumeBGM()
{
    if (!m_BGMComponent)
    {
        return;
    }

    // 既に再生中なら何もしない
    if (m_BGMComponent->IsPlaying())
    {
        return;
    }

    m_BGMComponent->Play();
}

//ループサウンドの再生
UAudioComponent* USoundManagerComponent::PlayLoopSound(USoundBase* Sound)
{
    if (!Sound)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("SoundManagerComponent::PlayLoopSound - Soundが設定されていません")
        );

        return nullptr;
    }

    // LoopSoundを生成して再生
    UAudioComponent* NewAudioComponent =
        UGameplayStatics::SpawnSound2D(
            this,
            Sound,
            1.0f,  //音量
            1.0f,  //音の高さ
            0.0f   //スタートタイム
        );

    if (NewAudioComponent)
    {
        // 管理対象に追加
        m_LoopSoundComponents.Add(NewAudioComponent);
    }

    return NewAudioComponent;
}

//ループサウンドの停止
void USoundManagerComponent::StopLoopSound(UAudioComponent* AudioComponent)
{
    if (!AudioComponent)
    {
        return;
    }

    AudioComponent->Stop();
}

//ループサウンドの再開
void USoundManagerComponent::ResumeLoopSound(UAudioComponent* AudioComponent)
{
    if (!AudioComponent)
    {
        return;
    }

    // 既に再生中なら何もしない
    if (AudioComponent->IsPlaying())
    {
        return;
    }

    AudioComponent->Play();
}

//サウンドの再生
void USoundManagerComponent::PlaySound(USoundBase* Sound)
{
    if (!Sound)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("SoundManagerComponent::PlaySound - Soundが設定されていません")
        );

        return;
    }

    // OneShotサウンド
    UGameplayStatics::SpawnSound2D(
        this,
        Sound, 
        1.0f,  // 音量
        1.0f   // 高さ
    );
}

//AnimNotify用再生関数
void USoundManagerComponent::PlayAnimNotifySound(
    USoundBase* Sound)
{
    if (!Sound)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("SoundManagerComponent::PlayAnimNotifySound - Soundが設定されていません")
        );

        return;
    }

    // AnimNotify専用関数だが、
    // 実際の再生処理は通常のSound再生に任せる
    PlaySound(Sound);
}
