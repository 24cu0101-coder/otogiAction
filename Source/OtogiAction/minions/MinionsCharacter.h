#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
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

	//死
	UFUNCTION()
	void Dead();

	//攻撃コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStatusComponent* StatusComponent;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};