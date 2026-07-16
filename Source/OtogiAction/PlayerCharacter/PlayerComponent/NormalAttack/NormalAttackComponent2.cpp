// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalAttackComponent2.h"

//コンストラクタ
UNormalAttackComponent2::UNormalAttackComponent2()
{
	PrimaryComponentTick.bCanEverTick = true;


	//キャラクター移動コンポーネント生成
	MCC = CreateDefaultSubobject<UMoveComponent>(TEXT("NAMC"));


}

void UNormalAttackComponent2::BeginPlay()
{
	Super::BeginPlay();



	AActor* OwnerActor = GetOwner();
	PlayerActor = Cast<APlayerCharacter>(OwnerActor);
	AbilitySystemComponent = PlayerActor->GetAbilitySystemComponent();

	if (!AbilitySystemComponent)
	{
		return;
	}

	//AbilitySystemComponent->InitAbilityActorInfo(GetOwner(), GetOwner());
	{
		if (GetOwner()->HasAuthority() && NAttackAbility1)
		{
			//プレイヤーにNAttackAbility1付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbility1, 1));
		}

		if (GetOwner()->HasAuthority() && NAttackAbility2)
		{
			//プレイヤーにNAttackAbility2付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbility2, 1));
		}

		if (GetOwner()->HasAuthority() && NAttackAbility3)
		{
			//プレイヤーにNAttackAbility3付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbility3, 1));
		}

		if (GetOwner()->HasAuthority() && KintaroAttack1)
		{
			//プレイヤーにNAttackAbility1付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(KintaroAttack1, 1));
		}

		if (GetOwner()->HasAuthority() && KintaroAttack2)
		{
			//プレイヤーにNAttackAbility2付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(KintaroAttack2, 1));
		}

		if (GetOwner()->HasAuthority() && KintaroAttack3)
		{
			//プレイヤーにNAttackAbility3付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(KintaroAttack3, 1));
		}
	}
}


//通常攻撃アビリティ実行(プレイヤーキャラクターで呼ばれる)
void UNormalAttackComponent2::ExecuteNormalAttackAbility()
{
	//タグ
	//入力が可能かどうかのタグ
	FGameplayTag PossibleNAttack = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.CantAttack"));

	//三段攻撃派生タグ
	FGameplayTag NextTag1 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag1"));
	FGameplayTag NextTag2 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag2"));

	//金太郎タグ
	FGameplayTag kintaroTag = FGameplayTag::RequestGameplayTag(TEXT("State.Skill.Kintaro"));
	//


	if (!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack))
	{

		return;
	}


	//金太郎タグがあったら
	if (AbilitySystemComponent->HasMatchingGameplayTag(kintaroTag))
	{	
		//アビリティがあるなら
		//現在のコンボ進行タグが2なら
		if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag2))
		{
			//アビリティがあるなら
			if (KintaroAttack3)
			{
				//アビリティ実行as
				AbilitySystemComponent->TryActivateAbilityByClass(KintaroAttack3);
				AbilitySystemComponent->RemoveLooseGameplayTag(NextTag2);
			}
		}

		//現在のコンボ進行タグが1なら
		else if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag1))
		{
			//アビリティがあるなら
			if (KintaroAttack2)
			{
				//アビリティ実行
				AbilitySystemComponent->TryActivateAbilityByClass(KintaroAttack2);
				AbilitySystemComponent->RemoveLooseGameplayTag(NextTag1);
			}
		}

		//入力が可能だったら
		else if (!AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack))
		{
			MCC->StartWarping(600.f);


			AbilitySystemComponent->TryActivateAbilityByClass(KintaroAttack1);
		}		
	}


	else
	{
		//現在のコンボ進行タグが2なら
		if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag2))
		{
			//アビリティがあるなら
			if (NAttackAbility3)
			{
				//アビリティ実行as
				AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility3);
				AbilitySystemComponent->RemoveLooseGameplayTag(NextTag2);
			}
		}

		//現在のコンボ進行タグが1なら
		else if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag1))
		{
			//アビリティがあるなら
			if (NAttackAbility2)
			{
				//アビリティ実行
				AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility2);
				AbilitySystemComponent->RemoveLooseGameplayTag(NextTag1);
			}
		}

		//入力が可能だったら
		else if (!AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack))
		{
			MCC->StartWarping(600.f);


			AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility1);
		}
	}
}


