
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PlayerDontAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UPlayerDontAttackNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	)override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerNotify.CantAttack")
	FGameplayTag DontAttackTag;

};
