#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MinionsAnimNotify.generated.h"

class UNiagaraSystem;

UCLASS()
class OTOGIACTION_API UMinionsAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    UNiagaraSystem* Effect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FName SocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FVector LocationOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FRotator RotationOffset = FRotator::ZeroRotator;

    virtual void Notify(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation) override;
};