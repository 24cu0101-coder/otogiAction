// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "NormalAttackComponent.generated.h"

//前方宣言
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UNormalAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNormalAttackComponent();

	//FGameplayTag 

protected:
	//プレイヤーのアクター
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;


	// Called when the game starts
	virtual void BeginPlay() override;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NormalAttack.Ability")
	UAbilitySystemComponent* AbilitySystemComponent;

	//通常攻撃のアビリティ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NormalAttack.Gas")
	TSubclassOf<UGameplayAbility> NAttackAbility1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NormalAttack.Gas")
	TSubclassOf<UGameplayAbility> NAttackAbility2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NormalAttack.Gas")
	TSubclassOf<UGameplayAbility> NAttackAbility3;



	//アビリティのハンドル
	FGameplayAbilitySpecHandle NAttackHandle;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//通常攻撃アビリティ起動関数
	void ExecuteNormalAttackAbility();

	//ノーティファイにASCを渡す関数(多分辞めた方がいい)
	UAbilitySystemComponent* GetNAttackACS()const {return AbilitySystemComponent; }

		
private:
	//キャラ移動コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NAttack.MovementComp", meta = (AllowPrivateAccess = "true"))
	UMoveComponent* MCC;


};
