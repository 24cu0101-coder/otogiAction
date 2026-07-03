#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_minionsAttack_Normal.generated.h"

UCLASS()
class OTOGIACTION_API UGA_minionsAttack_Normal : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_minionsAttack_Normal();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage = 20.f;
};