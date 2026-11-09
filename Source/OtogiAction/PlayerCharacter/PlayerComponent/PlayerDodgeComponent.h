// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "PlayerDodgeComponent.generated.h"

class UInputBufferComponent;
;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UPlayerDodgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerDodgeComponent();

	


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//プレイヤーのアクター
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dodge.Ability")
	UAbilitySystemComponent* AbilitySystemComponent;

	//回避のアビリティ
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Dodge.Gas")
	TSubclassOf<UGameplayAbility> DodgeAbility;

	//ジャスト回避のアビリティ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JustDodge.Gas")
	TSubclassOf<UGameplayAbility> JustDodgeAbility;

	//アビリティのハンドル
	FGameplayAbilitySpecHandle DodgeHandle;

	//アビリティのハンドル
	FGameplayAbilitySpecHandle JustDodgeHandle;



	UFUNCTION()
	void OnPlayerTakeDamage(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);


	UFUNCTION()
	FVector GetEnemyAngleVector(const AActor* EnemyActor);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//初期化時に呼ばれるabilityを付与する関数
	void DodgeAbilitySet(UAbilitySystemComponent* DodgeASC);

	//アビリティを呼び出す関数
	void ExecuteAbility();

	void ExecuteDodgeAbility();

	// ジャスト回避を呼び出す
	void ExecuteJustDodgeAbility();

	void JustDodgeWindow(const FGameplayTag Tag, int32 NewCount);

	FVector GetEnemyRote();


	//AbilitySystemInterfaceのゲッター関数
	UAbilitySystemComponent* GetDodgeAbilirtSystemComponent();

private:

	//先行入力のコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dodge.InputBufferComp", meta = (AllowPrivateAccess = "true"))
	UInputBufferComponent* DodgeInputBufferComp;

	FGameplayTag m_justDodgeTag;

	FVector EnemyRote;
		
};

