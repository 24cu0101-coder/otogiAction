#include "AnimNotifyState_MinionAttack.h"
#include "MinionsCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_MinionAttack::NotifyBegin(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    float TotalDuration,
    const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(
        MeshComp, Animation, TotalDuration, EventReference);

    if (!MeshComp) return;

    AMinionsCharacter* Minion =
        Cast<AMinionsCharacter>(MeshComp->GetOwner());

    if (Minion)
    {
        Minion->SetAttackInProgress(true);
    }
}

void UAnimNotifyState_MinionAttack::NotifyEnd(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(
        MeshComp, Animation, EventReference);

    if (!MeshComp) return;

    AMinionsCharacter* Minion =
        Cast<AMinionsCharacter>(MeshComp->GetOwner());

    if (Minion)
    {
        Minion->SetAttackInProgress(false);
    }
}