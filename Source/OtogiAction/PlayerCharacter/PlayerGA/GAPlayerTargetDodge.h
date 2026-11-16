#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAPlayerTargetDodge.generated.h"

/**
 * 
 */
// クラス宣言
class APlayerCharacter;		//プレイヤークラス
class UDamageType;
class UAbilityTask_WaitGameplayTagRemoved;
class UAbilityTask_ApplyRootMotionConstantForce;


UCLASS()
class OTOGIACTION_API UGAPlayerTargetDodge : public UGameplayAbility
{
	GENERATED_BODY()

public:

	

protected:

	// 回避のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* DodgeMontage;

	//プレイヤーの情報
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;


	//-------------------
	//関数
	//-------------------

	
};
