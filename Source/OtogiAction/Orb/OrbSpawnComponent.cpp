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

        float Radius = 300.f;

        FVector RandomOffset =
            FMath::VRand() * FMath::RandRange(50.f, Radius);

        RandomOffset.Z = FMath::RandRange(50.f, 200.f);

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