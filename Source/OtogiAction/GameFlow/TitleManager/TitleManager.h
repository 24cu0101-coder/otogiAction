// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleManager.generated.h"

//前方宣言
class ABookActor;
class AChainActor;

UCLASS()
class OTOGIACTION_API ATitleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATitleManager();

public:
    // レベル上に配置した各アクターの参照を設定
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Title Management")
    TObjectPtr<ABookActor> TargetBookActor;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Title Management")
    TObjectPtr<AChainActor> TargetChainActor;

    // 演出・遷移設定
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Settings")
    FName NextLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Settings")
    float FadeOutDuration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Settings")
    float FadeInDuration = 1.0f;

    // デリゲート受け取り用（UIまたはレベルBPから呼び出す）
    UFUNCTION(BlueprintCallable, Category = "Title Sequences")
    void OnButtonPressed();

private:
    void SwapBookAndFadeIn();
    void FinishSequenceAndOpenLevel();

    FTimerHandle TimerHandle;

};
