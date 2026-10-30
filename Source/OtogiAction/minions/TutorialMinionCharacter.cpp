// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialMinionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TutorialMinionCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/DamageType.h"

// Sets default values
ATutorialMinionCharacter::ATutorialMinionCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // コントローラーによる自動旋回を無効化
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // 移動と自動旋回を無効化
    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
}
void ATutorialMinionCharacter::BeginPlay()
{
    Super::BeginPlay();

    // ゲーム開始時に攻撃を再生
    PlayTutorialAttack();

    // 一定間隔で攻撃を繰り返す
    GetWorldTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &ATutorialMinionCharacter::PlayTutorialAttack,
        AttackInterval,
        true,
        AttackInterval
    );
}

void ATutorialMinionCharacter::PlayTutorialAttack()
{
    if (!AttackMontage)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("TutorialMinion: AttackMontage is not set"));
        return;
    }

    UAnimInstance* AnimInstance =
        GetMesh()->GetAnimInstance();

    if (!AnimInstance)
    {
        return;
    }

    if (AnimInstance->Montage_IsPlaying(AttackMontage))
    {
        return;
    }

    AnimInstance->Montage_Play(AttackMontage);
}

// Called every frame
void ATutorialMinionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATutorialMinionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATutorialMinionCharacter::ApplyTutorialAttackDamage()
{
    if (!GetWorld())
    {
        return;
    }

    // 敵の前方に攻撃判定を作る
    const FVector HitLocation =
        GetActorLocation()
        + GetActorForwardVector() * HitDistance
        + FVector(0.0f, 0.0f, 50.0f);

    // 球形の攻撃判定
    FCollisionShape Sphere =
        FCollisionShape::MakeSphere(HitRadius);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    TArray<FOverlapResult> OverlapResults;

    const bool bHit = GetWorld()->OverlapMultiByObjectType(
        OverlapResults,
        HitLocation,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECC_Pawn),
        Sphere,
        QueryParams
    );

    if (!bHit)
    {
        return;
    }

    // 判定内にいるプレイヤーへダメージ
    for (const FOverlapResult& Result : OverlapResults)
    {
        APlayerCharacter* Player =
            Cast<APlayerCharacter>(Result.GetActor());

        if (!Player)
        {
            continue;
        }

        UGameplayStatics::ApplyDamage(
            Player,
            AttackDamage,
            GetInstigatorController(),
            this,
            UDamageType::StaticClass()
        );

        UE_LOG(LogTemp, Warning,
            TEXT("Tutorial Minion hit Player! Damage: %f"),
            AttackDamage
        );
    }
}


void ATutorialMinionCharacter::DisappearWithEffect()
{
    if (bIsDying)
    {
        return;
    }

    bIsDying = true;

    // 攻撃タイマーを停止
    GetWorldTimerManager().ClearTimer(AttackTimerHandle);

    // 当たり判定を無効化
    SetActorEnableCollision(false);

    // HPバーがある場合は非表示
    TArray<UWidgetComponent*> WidgetComponents;
    GetComponents<UWidgetComponent>(WidgetComponents);

    for (UWidgetComponent* Widget : WidgetComponents)
    {
        if (Widget)
        {
            Widget->SetVisibility(false);
            Widget->SetHiddenInGame(true);
        }
    }

    // アニメーションを取得
    UAnimInstance* AnimInstance =
        GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

    if (!DisappearMontage || !AnimInstance)
    {
        // アニメーションが設定されていない場合
        // エフェクトを再生して削除
        OnDisappearMontageEnded(nullptr, false);
        return;
    }

    // アニメーション終了時の処理を登録
    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(
        this,
        &ATutorialMinionCharacter::OnDisappearMontageEnded
    );

    AnimInstance->Montage_SetEndDelegate(
        EndDelegate,
        DisappearMontage
    );

    // 消滅アニメーション再生
    const float Duration =
        AnimInstance->Montage_Play(DisappearMontage);

    if (Duration <= 0.0f)
    {
        OnDisappearMontageEnded(nullptr, false);
    }
}

void ATutorialMinionCharacter::OnDisappearMontageEnded(
    UAnimMontage* Montage,
    bool bInterrupted)
{
    if (!IsValid(this))
    {
        return;
    }

    // Niagaraエフェクトを再生
    if (DisappearEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            DisappearEffect,
            GetActorLocation(),
            GetActorRotation()
        );
    }

    // Actorを削除
    Destroy();
}