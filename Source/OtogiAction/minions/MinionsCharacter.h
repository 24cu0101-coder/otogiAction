#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Components/WidgetComponent.h"
#include "OtogiAction/Orb/OrbSpawnComponent.h"
#include "OtogiAction/Component/Audio/CharacterAudioComponent.h"
#include "MinionsCharacter.generated.h"

class UStatusComponent;
class UminionsAttackComponent;
class UWidgetComponent;
class USphereCollisionComponent;
class UMinionsHitReactionComponent;


UCLASS()
class OTOGIACTION_API AMinionsCharacter
	: public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMinionsCharacter();

protected:
	virtual void BeginPlay() override;

public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//====================
	// GAS
	//====================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	void GiveDefaultAbilities();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	//====================
	// Component
	//====================

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
	//====================
	// Orb
	//====================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb")
	int32 MaxOrbCount = 3;

	int32 SpawnedOrbCount = 0;

	//====================
	// Event
	//====================

	UFUNCTION()
	void OnDamage(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);

	UFUNCTION()
	void Dead();

	UFUNCTION()
	void UpdateHPWidget(float CurrentHP);

private:
	bool bIsHit = false;

public:
	bool GetIsHitFlg() const { return bIsHit; }
	void SetIsHitFlg(bool bHit) { bIsHit = bHit; }
};