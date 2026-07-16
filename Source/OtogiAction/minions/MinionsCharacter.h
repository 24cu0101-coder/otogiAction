/*#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "OtogiAction//Orb/OrbSpawnComponent.h"
#include "Components/WidgetComponent.h"
#include "MinionsCharacter.generated.h"

class UStatusComponent;
class UminionsAttackComponent;

UCLASS()
class OTOGIACTION_API AMinionsCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AMinionsCharacter();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//HPのwidget
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HPWidgetComponent;

	// スキル登録
	void GiveDefaultAbilities();

	// ブループリント側で設定する初期アビリティの配列
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	//ステータス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UminionsAttackComponent* AttackComponent;

	//ダメージを受け取る
	UFUNCTION() void OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);


	//死
	UFUNCTION()
	void Dead();

	//攻撃コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStatusComponent* StatusComponent;

	//オーブコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UOrbSpawnComponent* OrbSpawnComponent;

	// 出せるOrbの最大数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb")
	int32 MaxOrbCount = 3;

	// 現在出したOrb数
	int32 SpawnedOrbCount = 0;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};*/
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
};