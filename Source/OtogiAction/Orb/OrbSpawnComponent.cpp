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

AOrbActor* UOrbSpawnComponent::SpawnOrb()
{
    if (!OrbClass)
    {
        return nullptr;
    }

    FVector SpawnLocation =
        GetOwner()->GetActorLocation();

    SpawnLocation += FVector(
        FMath::RandRange(-150.f, 150.f),
        FMath::RandRange(-150.f, 150.f),
        150.f);

    AOrbActor* SpawnedOrb =
        GetWorld()->SpawnActor<AOrbActor>(
            OrbClass,
            SpawnLocation,
            FRotator::ZeroRotator);

    return SpawnedOrb;
}