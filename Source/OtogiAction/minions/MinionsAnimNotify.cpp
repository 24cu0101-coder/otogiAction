#include "MinionsAnimNotify.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

void UMinionsAnimNotify::Notify(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    if (!MeshComp || !Effect)
    {
        return;
    }

    UNiagaraFunctionLibrary::SpawnSystemAttached(
        Effect,
        MeshComp,
        SocketName,
        LocationOffset,
        RotationOffset,
        EAttachLocation::KeepRelativeOffset,
        true);
}