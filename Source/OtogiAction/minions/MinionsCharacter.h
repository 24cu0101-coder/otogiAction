#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "OtogiAction//Orb/OrbSpawnComponent.h"
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

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};