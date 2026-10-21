//担当：飯島

#include "CameraTargetComponent.h"
#include "../PlayerTargetComponent.h"
#include "../../PlayerCharacter.h"
#include "GameFramework/Actor.h"

//コンストラクタ
UCameraTargetComponent::UCameraTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PlayerTargetComponent = nullptr;
}


//生成されたら処理される
void UCameraTargetComponent::BeginPlay()
{
	Super::BeginPlay();

    AActor* OwnerActor = GetOwner();

    if (!OwnerActor) return;

    PlayerTargetComponent = OwnerActor->FindComponentByClass<UPlayerTargetComponent>();
}

//アクターを取得
AActor* UCameraTargetComponent::GetTargetActor() const
{
	if (!PlayerTargetComponent) { return nullptr; }

    return PlayerTargetComponent->GetCurrentTargetActor();
}

//通常時のターゲット注視点を取得
FVector UCameraTargetComponent::GetDefaultTargetPoint() const
{
    const AActor* OwnerActor = GetOwner();

    if (!OwnerActor)
    {
        return FVector::ZeroVector;
    }

    FVector Result = OwnerActor->GetActorLocation();

    Result.Z += PlayerFocusHeight;

    return Result;
}

//ターゲット時の注視点を取得
FVector UCameraTargetComponent::GetLockOnTargetPoint() const
{
    const AActor* OwnerActor = GetOwner();

    if (!OwnerActor)
    {
        return FVector::ZeroVector;
    }

    AActor* TargetActor = GetTargetActor();

    if (!TargetActor)
    {
        return GetDefaultTargetPoint();
    }

    FVector PlayerPoint = OwnerActor->GetActorLocation();
    PlayerPoint.Z += PlayerFocusHeight;

    FVector EnemyPoint = TargetActor->GetActorLocation();
    EnemyPoint.Z += EnemyFocusHeight;

    const float ClampedWeight =
        FMath::Clamp(EnemyWeight, 0.0f, 1.0f);

    const float PlayerWeight =
        1.0f - ClampedWeight;

    return
        PlayerPoint * PlayerWeight +
        EnemyPoint * ClampedWeight;
}



