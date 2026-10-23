#include "AnimNotify_TutorialMinionAttack.h"
#include "TutorialMinionCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_TutorialMinionAttack::Notify(
    USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation,
    const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (!MeshComp)
    {
        return;
    }

    ATutorialMinionCharacter* Minion =
        Cast<ATutorialMinionCharacter>(MeshComp->GetOwner());

    if (!Minion)
    {
        return;
    }

    // Notifyを置いた瞬間に攻撃判定を1回実行
    Minion->ApplyTutorialAttackDamage();
}