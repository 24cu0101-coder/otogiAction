#include "OrbSpawnComponent.h"
#include "OrbActor.h"

UOrbSpawnComponent::UOrbSpawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UOrbSpawnComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UOrbSpawnComponent::SpawnOrbs(AActor* OwnerEnemy, float Damage)
{
    if (!OrbClass || !OwnerEnemy)
    {
        return;
    }

    float RemainingDamage = Damage;

    while (RemainingDamage > 0.f &&
        CurrentOrbCount < MaxOrbSpawn)
    {
        float OrbDamage = FMath::Min(10.f, RemainingDamage);

        // 敵を中心に、ランダムな位置へOrbを生成
        float MinRadius = 100.f;
        float MaxRadius = 600.f;

        // 水平方向の角度をランダムに決める
        float Angle = FMath::RandRange(0.f, 360.f);

        // 敵からの距離をランダムに決める
        float Radius = FMath::RandRange(MinRadius, MaxRadius);

        // X・Y方向に大きく散らばらせる
        FVector RandomOffset(
            FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius,
            FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius,
            FMath::RandRange(50.f, 350.f)
        );
        FVector SpawnLocation =
            OwnerEnemy->GetActorLocation() + RandomOffset;

        AOrbActor* SpawnedOrb =
            GetWorld()->SpawnActor<AOrbActor>(
                OrbClass,
                SpawnLocation,
                FRotator::ZeroRotator);

        if (SpawnedOrb)
        {
            SpawnedOrb->SetOwnerEnemy(OwnerEnemy);

            SpawnedOrb->SetOrbDamage(OrbDamage);

            // このコンポーネントをOrbへ渡す
            SpawnedOrb->SetSpawnComponent(this);

            CurrentOrbCount++;
        }

        RemainingDamage -= OrbDamage;
    }
}

void UOrbSpawnComponent::ReturnOrb()
{
    CurrentOrbCount = FMath::Max(0, CurrentOrbCount - 1);
}