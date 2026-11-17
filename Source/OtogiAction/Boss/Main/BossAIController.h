// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
    // ControllerがPawn（Character）を所有（Possess）した時に自動で呼ばれるイベント
    virtual void OnPossess(APawn* InPawn) override;

    // ControllerがPawnから離脱（Unpossess）した時に呼ばれるイベント
    virtual void OnUnPossess() override;

    // エディタ上からBehaviorTreeアセットを指定するための変数
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
};
