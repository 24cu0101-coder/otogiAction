// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "AbilitySystemComponent.h"
#include "SkillGaugeComponent.h"


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


	for (const FSkillSet& Group : SkillGroups)
	{
		if (Group.SkillAbility)
		{
			OwnerASC->GiveAbility(FGameplayAbilitySpec(Group.SkillAbility, 1, 0));
		}
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
	//呼ばれてるかのログ
	UE_LOG(LogTemp, Warning, TEXT(" Called RequestSkillTrigger(ButtonIndex: % d)"), ButtonIndex);	
	if (!OwnerASC || SkillGroups.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed: OwnerASC is NULL or SkillGroups is Empty!"));
		return;
	}
	FSkillSet CurrentSet = SkillGroups[CurrentGroupPointer];

	if (!CurrentSet.SkillAbility)
	{
		UE_LOG(LogTemp, Error, TEXT("× Failed: CurrentSet.SkillAbility is NULL!"));
		return;
	}

	//重複発動を防ぐ
	FGameplayTag ExcuteSkillTag = FGameplayTag::RequestGameplayTag(FName("State.Attacking.Skill"));
	FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(FName("State.Attacking"));
	FGameplayTag KintaroSkillTag = FGameplayTag::RequestGameplayTag(FName("State.Skill.Kintaro"));
	//if (OwnerASC->HasMatchingGameplayTag(ExcuteSkillTag) || OwnerASC->HasMatchingGameplayTag(KintaroSkillTag) || OwnerASC->HasMatchingGameplayTag(AttackTag)) return;

	if (OwnerASC->HasMatchingGameplayTag(ExcuteSkillTag))
	{
		UE_LOG(LogTemp, Error, TEXT("ErrorSkillExcuteTagNameExcute"));
		return;
	}
	if (OwnerASC->HasMatchingGameplayTag(KintaroSkillTag))
	{
		UE_LOG(LogTemp, Error, TEXT("ErrorSkillExcuteTagNameKintaro"));
		return;
	}
	if (OwnerASC->HasMatchingGameplayTag(AttackTag))
	{
		UE_LOG(LogTemp, Error, TEXT("ErrorSkillExcuteTagNameAttack"));
		return;
	}

	//スキルゲージのチェックと消費
	if (USkillGaugeComponent* GaugeComp = GetOwner()->FindComponentByClass<USkillGaugeComponent>())
	{
		//ゲージが足りているか
		if (GaugeComp->CanUseSkill(CurrentSet.Cost))
		{
			UE_LOG(LogTemp, Warning, TEXT("OkSkillGauge"));
			//消費に成功したらアビリティを発動
			if (GaugeComp->ConsumeGauge(CurrentSet.Cost))
			{
				UE_LOG(LogTemp, Warning, TEXT("ExcuteSkill"));
				OwnerASC->TryActivateAbilityByClass(CurrentSet.SkillAbility);
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning, TEXT("NoSkillGauge"));
		}
	}
}

