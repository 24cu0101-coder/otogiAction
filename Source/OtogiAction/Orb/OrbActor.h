#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbActor.generated.h"

class UStaticMeshComponent;
class UOrbSpawnComponent;

UCLASS()
class OTOGIACTION_API AOrbActor : public AActor
{
    GENERATED_BODY()

public:
    AOrbActor();

    void SetOwnerEnemy(AActor* Enemy);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void StartAbsorb(AActor* Target);

    void SetSpawnComponent(UOrbSpawnComponent* InComponent);

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY()
    AActor* OwnerEnemy = nullptr;

    UPROPERTY()
    UOrbSpawnComponent* SpawnComponent;

private:
    FVector StartLocation;
    float FloatTime = 0.f;

    UPROPERTY(EditAnywhere, Category = "Orb")
    float FloatHeight = 30.f;

    UPROPERTY(EditAnywhere, Category = "Orb")
    float FloatSpeed = 2.f;

    // 自動九州の距離
    UPROPERTY(EditAnywhere, Category = "Orb")
    float AbsorbRange = 300.f;

    // 吸われているか
    bool bIsAbsorbing = false;

    // 向かう相手
    UPROPERTY()
    AActor* TargetActor = nullptr;

    UPROPERTY()
    float OrbDamage = 0.f;

public:

    void SetOrbDamage(float InDamage);

};