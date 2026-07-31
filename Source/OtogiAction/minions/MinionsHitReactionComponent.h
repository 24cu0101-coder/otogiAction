#pragma once

#include "OtogiAction/Component/UHitReactionBaseComponent.h"
#include "CoreMinimal.h"
#include "MinionsHitReactionComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UMinionsHitReactionComponent
	: public UUHitReactionBaseComponent
{

	GENERATED_BODY()


public:

	UMinionsHitReactionComponent();


	// 姿勢崩壊時の大きい怯み
	virtual void PlayHitReaction(float DamageAmount) override;


protected:

	// BaseComponent用
	virtual void OnFearMax() override;


	// スタン時
	virtual void OnStunMax() override;



public:

	//====================
	// Stance
	//====================

	void AddStance(float Value);


	bool IsStanceBreak() const;


	void ResetStance();



	//====================
	// Hit方向
	//====================

	void SetHitDirection(AActor* Attacker);



	//====================
	// Stance状態
	//====================

	void SetStanceBroken(bool Value);


	bool IsStanceBroken() const;



private:


	//====================
	// Stance
	//====================

	float CurrentStance = 100.f;


	UPROPERTY(EditAnywhere, Category = "Stance")
	float MaxStance = 100.f;



	bool bStanceBroken = false;



	//====================
	// Hit Direction
	//====================

	FVector HitDirection;



	//====================
	// Stun
	//====================

	void StopMovement();


	void StopAI();


	void RestartAI();


	void RecoverFromStun();



	FTimerHandle StunRecoverTimerHandle;



	UPROPERTY(EditAnywhere, Category = "Stun")
	float StunDownDuration = 4.f;



	//====================
	// Montage
	//====================


	// 大きい正面Hit
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* FrontHitMontage;


	// 大きい背面Hit
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* BackHitMontage;



	// 軽い正面Hit
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* LightFrontHitMontage;


	// 軽い背面Hit
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* LightBackHitMontage;



	// スタン倒れ
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* StunKnockdownMontage;

	// 起き上がりMontage
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* GetUpMontage = nullptr;

	// 軽いHit発生確率
	UPROPERTY(EditAnywhere, Category = "Reaction")
	float LightHitChance = 0.35f;

	// 軽いHit再生
	void TryPlayLightHit();



	// 攻撃キャンセル用
	void CancelAttack();

	private:

		void FinishRecover();

		FTimerHandle GetUpTimerHandle;

};