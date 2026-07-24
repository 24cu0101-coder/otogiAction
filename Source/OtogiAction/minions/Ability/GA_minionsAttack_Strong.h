#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Animation/AnimMontage.h"
#include "GA_minionsAttack_Strong.generated.h"


UCLASS()
class OTOGIACTION_API UGA_minionsAttack_Strong : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_minionsAttack_Strong();


	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;


protected:

	// 攻撃Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;


	// ダメージ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage = 30.f;


	// Montage終了
	UFUNCTION()
	void OnMontageCompleted();


	UFUNCTION()
	void OnMontageInterrupted();


private:

	FGameplayAbilitySpecHandle CurrentSpecHandle;

	const FGameplayAbilityActorInfo* CurrentActorInfo;

	FGameplayAbilityActivationInfo CurrentActivationInfo;

};