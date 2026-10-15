
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialPoint.generated.h"

class UBoxComponent;
class ATutorialActor;

UCLASS()
class OTOGIACTION_API ATutorialPoint : public AActor
{
    GENERATED_BODY()

public:
    ATutorialPoint();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tutorial")
    TObjectPtr<UBoxComponent> TriggerBox;

    // 何番目の目的地か（最初は0）
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Tutorial")
    int32 StepIndex = 0;

    // チュートリアル管理Actor
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Tutorial")
    TObjectPtr<ATutorialActor> TutorialManager;

    UFUNCTION()
    void OnTriggerBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
