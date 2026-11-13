#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Components/WidgetComponent.h"
#include "OtogiAction/Orb/OrbSpawnComponent.h"
#include "OtogiAction/Component/Audio/CharacterAudioComponent.h"
#include "NiagaraSystem.h"
#include "MinionsCharacter.generated.h"

class UStatusComponent;
class UminionsAttackComponent;
class UWidgetComponent;
class USphereCollisionComponent;
class UMinionsHitReactionComponent;
class UAnimMontage;
class UNiagaraSystem;


UCLASS()
class OTOGIACTION_API AMinionsCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMinionsCharacter();

protected:
	virtual void BeginPlay() override;

	FTimerHandle DeathTimerHandle;

	void CheckShieldGuardInput();
public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// GAS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	void GiveDefaultAbilities();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UminionsAttackComponent* AttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UOrbSpawnComponent* OrbSpawnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* HPWidgetComponent;

	//Audio
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCharacterAudioComponent* CharacterAudioComponent;

	UPROPERTY()
	USphereCollisionComponent* SphereCollisionComponent;

	//HitReaction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMinionsHitReactionComponent* HitReactionComponent;


	UPROPERTY(BlueprintReadOnly)
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, category = "Effect")
	UNiagaraSystem* HitEffect;

	//SpecialAbility
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Enemy")
	bool bKintaroOnlyEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	bool bCanTakeDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "HP Widget", meta = (AllowPrivateAccess = "true"))
	float HPWidgetVisibleDistance = 1500.0f;

	void SetCanTakeDamage(bool bEnable)
	{
		bCanTakeDamage = bEnable;


	}

	// Orb状態
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb")
	bool bCanSpawnOrb = false;

	//攻撃速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackPlayRate = 1.0f;

	// Minionごとの攻撃力
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Status")
	float AttackPower = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool bTutorialNoDamage = false;


	//盾のメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Shield")
	TObjectPtr<UStaticMeshComponent>ShieldMesh;

	//盾を持っているか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	bool bHasShield = false;

	// 入力監視タイマー
	FTimerHandle ShieldInputTimerHandle;

	// 前回の入力状態
	bool bWasShieldInputPressed = false;
public:

	void CancelAttack();
	void SetIsAttacking(bool Value)
	{
		bIsAttacking = Value;
	}


	bool IsAttacking() const
	{
		return bIsAttacking;
	}

	// Event
	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void Dead();

	UFUNCTION()
	void UpdateHPWidget(float CurrentHP);

	void SetCanSpawnOrb(bool bEnable);

	void UpdateHPWidgetVisibility();

	// 攻撃中NotifyStateから呼び出す
	void SetAttackInProgress(bool bInProgress);

	// 攻撃アニメーション中かどうか
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsAttackInProgress = false;

	// 死亡アニメーション
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	TObjectPtr<UAnimMontage> DeathMontage;

	// 死亡時のNiagaraエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	TObjectPtr<UNiagaraSystem> DeathEffect;

	// エフェクトの大きさ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death",
		meta = (ClampMin = "0.1"))
	float DeathEffectScale = 1.0f;

	// エフェクト再生時間（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death",
		meta = (ClampMin = "0.1"))
	float DeathEffectDuration = 2.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnDead();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void OnDeathAnimationFinished();

	UPROPERTY(EditAnywhere, Category = "Death")
	float DeathEffectDestroyDelay = 2.0f;


	
	//盾ガードすr範囲
	UPROPERTY(EditAnywhere, Category = "Shield")
	float ShieldGuardRange = 300.0f;

	// UPROPERTY として追加（ブループリントから耐久値を調整できるようにする）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float MaxShieldDurability = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	float CurrentShieldDurability = 100.f;


	// 通常攻撃で削れる盾の耐久値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float NormalAttackShieldDamage = 1.f;

	// KintaroSkillで削れる盾の耐久値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	float KintaroSkillShieldDamage = 50.f;

	// KintaroSkill判定用のDamageTypeクラス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	TSubclassOf<UDamageType> KintaroSkillDamageType;

	// 金太郎スキル中かどうかのフラグ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
	bool bIsKintaroSkillActive = false;

	// 金太郎スキル状態をセットする関数
	void SetKintaroSkillActive(bool bActive) { bIsKintaroSkillActive = bActive; }

	// 盾破壊処理
	void BreakShield();

	// 盾でガード成功したときのエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	TObjectPtr<UNiagaraSystem> ShieldGuardEffect;

	// 盾が壊れたときのエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	TObjectPtr<UNiagaraSystem> ShieldBreakEffect;


	// HitReactionComponentのゲッター（攻撃アビリティ側でも使うため）
	UMinionsHitReactionComponent* GetHitReactionComponent() const { return HitReactionComponent; }

	// 死亡演出用の関数
	void PlayDeathEffect();
	void FinishDeath();

	// 攻撃中の回転をロックする
	void SetAttackRotationLock(bool bLock);


private:
	bool bIsHit = false;

	bool bOriginalUseControllerDesiredRotation = true;	

public:
	bool GetIsHitFlg() const { return bIsHit; }
	void SetIsHitFlg(bool bHit) { bIsHit = bHit; }

	bool IsDead() const
	{
		return bIsDead;
	}



private:

	bool bIsDead = false;
};