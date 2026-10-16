// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleManager.generated.h"

//前方宣言
class ABookActor;
class AChainActor;
class USoundManagerComponent;

UCLASS()
class OTOGIACTION_API ATitleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATitleManager();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USoundManagerComponent* SoundManager;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Title Management")
    ABookActor* TargetBookActor;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Title Management")
    AChainActor* TargetChainActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Settings")
    USoundBase* TitleBGM;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Settings")
    FName NextLevelName;

    // 本が開いてからホワイトアウトが始まるまでの待ち時間（演出を見せる時間）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Settings")
    float HoldBeforeFadeDuration = 1.0f;

    // ホワイトアウトにかける時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Settings")
    float FadeOutDuration = 1.0f;

    // ホワイトアウトを維持する時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Settings")
    float FadeOutKeepDuration = 3.0f;

    UFUNCTION(BlueprintCallable, Category = "Title Sequences")
    void OnButtonPressed();

private:
    void StartWhiteout();
    void FinishSequenceAndOpenLevel();

    FTimerHandle TimerHandle;

};
