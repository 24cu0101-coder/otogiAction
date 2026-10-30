#include "TutorialMinionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/DamageType.h"

// コンストラクタ
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

void ATutorialMinionCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATutorialMinionCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
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

    // HPバーを非表示
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

    UAnimInstance* AnimInstance =
        GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

    if (!DisappearMontage || !AnimInstance)
    {
        UE_LOG(LogTemp, Error,
            TEXT("DisappearMontageまたはAnimInstanceがありません"));

        OnDisappearMontageEnded(nullptr, false);
        return;
    }

    // 先にアニメーションを再生
    const float Duration =
        AnimInstance->Montage_Play(DisappearMontage);

    if (Duration <= 0.0f)
    {
        UE_LOG(LogTemp, Error,
            TEXT("DisappearMontageの再生に失敗しました"));

        OnDisappearMontageEnded(nullptr, false);
        return;
    }

    // 再生後に終了通知を登録
    FOnMontageEnded EndDelegate;

    EndDelegate.BindUObject(
        this,
        &ATutorialMinionCharacter::OnDisappearMontageEnded
    );

    AnimInstance->Montage_SetEndDelegate(
        EndDelegate,
        DisappearMontage
    );

    UE_LOG(LogTemp, Warning,
        TEXT("DisappearMontage再生開始：%f秒"),
        Duration);
}

void ATutorialMinionCharacter::OnDisappearMontageEnded(
    UAnimMontage* Montage,
    bool bInterrupted)
{
    UE_LOG(LogTemp, Warning,
        TEXT("① OnDisappearMontageEnded 呼び出し！"));

    if (!IsValid(this))
    {
        return;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("② SpawnDisappearEffect 呼び出し前"));

    SpawnDisappearEffect();

    UE_LOG(LogTemp, Warning,
        TEXT("③ SpawnDisappearEffect 呼び出し後"));

    Destroy();

    UE_LOG(LogTemp, Warning,
        TEXT("④ Destroy実行"));
}