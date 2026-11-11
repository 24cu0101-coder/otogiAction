// Fill out your copyright notice in the Description page of Project Settings.


#include "BossChaseComponent.h"
#include "BossAIController.h"
#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBossChaseComponent::UBossChaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossChaseComponent::BeginPlay()
{
	Super::BeginPlay();

    //所有者（ボス）のAIControllerを取得
    AActor* Owner = GetOwner();
    if (Owner)
    {
        //Owner自身がAPawnか確認し、そのControllerを取得
        APawn* OwnerPawn = Cast<APawn>(Owner);
        if (OwnerPawn)
        {
            CachedAIController = Cast<ABossAIController>(OwnerPawn->GetController());
        }
    }

    //ターゲット（プレイヤー）の検索・保持
    SetTargetPlayer();
}

// Called every frame
void UBossChaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsTrackingEnable || !CachedAIController || !TargetActor)
    {
        return;
    }

    // Tickごとの実行を避け、一定間隔で移動命令を更新する
    TimerAccumulator += DeltaTime;
    if (TimerAccumulator >= UpdateInterval)
    {
        TimerAccumulator = 0.0f;
        TrackingPlayer();
    }
}

// 追跡機能の有効化 / 無効化切り替え用
void UBossChaseComponent::SetTrackingEnabled(bool bEnable)
{
    bIsTrackingEnable = bEnable;

    // 無効化された時は追跡移動を停止させる
    if (!bIsTrackingEnable && CachedAIController)
    {
        CachedAIController->StopMovement();
    }
}

//発見したプレイヤーをセットする
void UBossChaseComponent::SetTargetPlayer()
{
    // 0番目のプレイヤーキャラを取得してターゲットに設定
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        TargetActor = Cast<ABossCharacter>(PlayerPawn);
    }
}

//追跡処理の実行
void UBossChaseComponent::TrackingPlayer()
{
    if (CachedAIController && TargetActor)
    {
        // AIControllerのMoveToActorを呼び出し、ナビメッシュ経由でプレイヤーを追尾
        CachedAIController->MoveToActor(
            TargetActor,
            50,
            true,   // bStopOnOverlap: ターゲットと重なった時に止まるか
            true,   // bUsePathfinding: ナビメッシュを使用した経路探索を行うか
            true,   // bCanStrafe: 移動中の旋回を許可するか
            nullptr,// FilterClass
            true    // bAllowPartialPath: 目的地への完全なパスがない場合も可能な限り近づくか
        );
    }
}

