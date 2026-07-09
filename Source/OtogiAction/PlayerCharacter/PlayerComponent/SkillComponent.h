// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilities/GameplayAbility.h"
#include "SkillComponent.generated.h"

//前方宣言
class UAbilitySystemComponent;

//スキル群の内容をセットする構造体
USTRUCT(BlueprintType)
struct FSkillSet
{
	GENERATED_BODY()

	//1つ目のスキル
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility>Skill_A;

	//2つ目のスキル
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility>Skill_B;

	//3つ目のスキル
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility>Skill_X;

	//4つ目のスキル
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility>Skill_Y;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

	//スキル群の配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<FSkillSet> SkillGroups;

private:
	//このコンポーネントを取り付け先のAbilitySystem
	UPROPERTY()
	UAbilitySystemComponent* OwnerASC;

	//現在選択中のスキル群を指すインデックス
	int32 CurrentGroupPointer = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	//ゲームが始まったときに呼ばれるキャラクターとGASの紐づけ
	void RegisterAbilities(UAbilitySystemComponent* ASC);

	//スキル群の選択
	void SwitchSkillGroup(int32 Direction);

	//押したボタンによってスキルを発動する
	void RequestSkillTrigger(int32 ButtonIndex);

};
