#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbSpawnComponent.generated.h"

class AOrbActor;
class UStatusComponent;

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

    // ダメージ分のオーブを生成
    void SpawnOrbs(AActor* OwnerEnemy,float Damage);
};