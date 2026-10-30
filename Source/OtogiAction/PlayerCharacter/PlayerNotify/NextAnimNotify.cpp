

//--------------------------------------------------------------------------------------------------------
//つぎの攻撃のタグを渡す通知
//-----------------------------------------------------------------------------------------------------

#include "NextAnimNotify.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/NormalAttack/NormalComboAttackComponent.h"

void UNextAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UE_LOG(LogTemp, Warning, TEXT("【UNextAnimNotify.cpp】: ddd"));


	if (!MeshComp) return;

	UE_LOG(LogTemp, Warning, TEXT("【UNextAnimNotify.cpp】: MeshComp"));


	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("【UNextAnimNotify.cpp】: OwnerActor"));


		if (UNormalComboAttackComponent* NAttackComp = OwnerActor->FindComponentByClass<UNormalComboAttackComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("【UNextAnimNotify.cpp】: NAttackComp"));


			if (UAbilitySystemComponent* ASComponent = NAttackComp->GetNAttackACS())
			{
				UE_LOG(LogTemp, Warning, TEXT("【UNextAnimNotify.cpp】: ASComponent"));


				ASComponent->AddLooseGameplayTag(NextNAttackTag);
			}
		}
	}
}

void UNextAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (UNormalComboAttackComponent* NAttackComp = Owner->FindComponentByClass<UNormalComboAttackComponent>())
		{
			if (UAbilitySystemComponent* ASC = NAttackComp->GetNAttackACS())
			{
				ASC->RemoveLooseGameplayTag(NextNAttackTag);
			}
		}
	}
}

