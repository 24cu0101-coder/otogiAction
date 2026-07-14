
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAStrongAttack.generated.h"

//クラス前方宣言

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UGAStrongAttack : public UGameplayAbility
{
	GENERATED_BODY()	

public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle SAttack,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;

protected:
	
	//強攻撃のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* SAttackMontage;

	//プレイヤーの情報
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* ASC;


	//モンタージュを再生する関数
	UFUNCTION()
	void PlaySAttackMontage();

	UFUNCTION()
	void RestartSAttackMontage();


	UFUNCTION()
	void SAttackMontageEnd();

	UFUNCTION()
	void SAttackAbilityEnd();
};
