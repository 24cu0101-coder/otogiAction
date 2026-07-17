// Fill out your copyright notice in the Description page of Project Settings.


#include "StrongAttackComponent3.h"
//UStrongAttackComponent3::UStrongAttackComponent3()
//{
//	PrimaryComponentTick.bCanEverTick = true;
//
//	//キャラクター移動コンポーネント生成
//	MCC = CreateDefaultSubobject<UMoveComponent>(TEXT("SA3MC"));
//}
//
//
//void UStrongAttackComponent3::BeginPlay()
//{
//
//	Super::BeginPlay();
//
//	AActor* OwnerActor = GetOwner();
//	PlayerActor = Cast<APlayerCharacter>(OwnerActor);
//	AbilitySystemComponent = PlayerActor->GetAbilitySystemComponent();
//
//	if (!AbilitySystemComponent)
//	{
//		return;
//	}
//
//	//アビリティを付与
//	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(SAttackAbility2, 1));
//}
//
//
//void UStrongAttackComponent3::ExecuteSutrongAttack3Ability()
//{
//	//タグ
//	//入力が可能かどうかのタグ
//	FGameplayTag PossibleNAttack = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.CantAttack"));
//
//	//三段攻撃派生タグ
//	FGameplayTag NextTag1 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag1"));
//	FGameplayTag NextTag2 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag2"));
//
//	//金太郎タグ
//	FGameplayTag kintaroTag = FGameplayTag::RequestGameplayTag(TEXT("State.Skill.Kintaro"));
//
//	FGameplayTag SkillAttack = FGameplayTag::RequestGameplayTag(TEXT("State.Attacking.Skill"));
//
//	//ヌルチェック
//	if (!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack) || AbilitySystemComponent->HasMatchingGameplayTag(SkillAttack))
//	{
//		return;
//	}
//
//
//
//
//
//}

void UStrongAttackComponent3::ExecuteSutrongAttack3Ability()
{

}