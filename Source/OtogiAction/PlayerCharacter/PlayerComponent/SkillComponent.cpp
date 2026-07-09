// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//スキル群に入っているGASの登録
void USkillComponent::RegisterAbilities(UAbilitySystemComponent* ASC)
{
	OwnerASC = ASC;

	//ACSがNULLなら返す
	if (!OwnerASC)return;

	//登録されているAbilityにGASを登録
	for (const FSkillSet& Group : SkillGroups)
	{
		if (Group.Skill_A) OwnerASC->GiveAbility(FGameplayAbilitySpec(Group.Skill_A, 1, 0));
		if (Group.Skill_B) OwnerASC->GiveAbility(FGameplayAbilitySpec(Group.Skill_B, 1, 0));
		if (Group.Skill_X) OwnerASC->GiveAbility(FGameplayAbilitySpec(Group.Skill_X, 1, 0));
		if (Group.Skill_Y) OwnerASC->GiveAbility(FGameplayAbilitySpec(Group.Skill_Y, 1, 0));
	}
}

//スキル群の選択
void USkillComponent::SwitchSkillGroup(int32 Direction)
{
	//ACSがある且つ登録されているスキル群がある場合を除いて返す。
	if (!OwnerASC || SkillGroups.Num() == 0)return;

	//配列の範囲でループしてスキル群を切り替える。
	CurrentGroupPointer = (CurrentGroupPointer + Direction + SkillGroups.Num()) % SkillGroups.Num();

	//現在のスキル群をログで出力
	UE_LOG(LogTemp, Log, TEXT("Current Skill Group pointer %d"), CurrentGroupPointer);
}

void USkillComponent::RequestSkillTrigger(int32 ButtonIndex)
{
	//ACSがある且つ登録されているスキル群がある場合を除いて返す。
	if (!OwnerASC || SkillGroups.Num() == 0)return;

	//選択中のスキル群に入っているスキルを取得
	FSkillSet CurrentSet = SkillGroups[CurrentGroupPointer];
	TSubclassOf<UGameplayAbility>TargetSkillAbilityClass = nullptr;

	//ボタンのインデックスに対応したアビリティをターゲットにセット
	switch (ButtonIndex)
	{
	case 0:
		TargetSkillAbilityClass = CurrentSet.Skill_A;
		break;

	case 1:
		TargetSkillAbilityClass = CurrentSet.Skill_B;
		break;

	case 2:
		TargetSkillAbilityClass = CurrentSet.Skill_X;
		break;

	case 3:
		TargetSkillAbilityClass = CurrentSet.Skill_Y;
		break;

	default:
		break;
	}

	FGameplayTag ExcuteSkillTag = FGameplayTag::RequestGameplayTag(FName("State.Attacking.Skill"));
	if (OwnerASC->HasMatchingGameplayTag(ExcuteSkillTag)) return;

	//スキルの発動
	if (TargetSkillAbilityClass)
	{
		OwnerASC->TryActivateAbilityByClass(TargetSkillAbilityClass);
	}
}

