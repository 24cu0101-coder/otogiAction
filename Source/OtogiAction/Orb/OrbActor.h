#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbActor.generated.h"

class UStaticMeshComponent;

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
private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY()
    AActor* OwnerEnemy=nullptr;
private:
    FVector StartLocation;
    float FloatTime = 0.f;

    UPROPERTY(EditAnywhere, Category = "Orb")
    float FloatHeight = 30.f;

    UPROPERTY(EditAnywhere,Category = "Orb")
    float FloatSpeed = 2.f;

    // ‹z‚í‚ê‚Ä‚¢‚é‚©
    bool bIsAbsorbing = false;

    // Œü‚©‚¤‘ŠŽè
    UPROPERTY()
    AActor* TargetActor = nullptr;

};