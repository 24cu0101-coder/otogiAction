#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbSpawnComponent.generated.h"

class AOrbActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UOrbSpawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UOrbSpawnComponent();

protected:
    virtual void BeginPlay() override;

public:

    UPROPERTY(EditAnywhere, Category = "Orb")
    TSubclassOf<AOrbActor> OrbClass;

    // 最大生成数
    UPROPERTY(EditAnywhere, Category = "Orb")
    int32 MaxOrbSpawn = 10;

    // 現在生成されている数
    UPROPERTY(VisibleAnywhere, Category = "Orb")
    int32 CurrentOrbCount = 0;

    void SpawnOrbs(AActor* OwnerEnemy, float Damage);

    // Orbが消えた時に呼ぶ
    void ReturnOrb();
};