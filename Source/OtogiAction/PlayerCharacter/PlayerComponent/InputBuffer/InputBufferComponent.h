// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "InputBufferComponent.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UInputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInputBufferComponent();

protected:
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//関数を保存、もしくは実行する関数(各コンポーネントで呼び出す　引数には各アクションを実行する関数を入れる)
	void KeepOrExeFunction(TFunction<void()> Function);


private:

	UFUNCTION()
	bool InputBufferFlag();

	//プレイヤーのアクター
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, Category = "InputBuffer.Ability")
	UAbilitySystemComponent* ASC;


	//保存していた関数を実行する関数(タグを検知して呼び出す)
	void ExecuteBoffereFunction(const FGameplayTag Tag, int32 NewCount);

	//関数を保存する変数
	TFunction<void()> ExeFunction;

	//次に発動する関数を入れる変数
	TFunction<void()> NextFunction;

};
