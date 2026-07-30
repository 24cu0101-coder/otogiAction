// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalComboAttackComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/Move/MoveComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/InputBuffer/InputBufferComponent.h"


//コンストラクタ
UNormalComboAttackComponent::UNormalComboAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


	//キャラクター移動コンポーネント生成
	MCC = CreateDefaultSubobject<UMoveComponent>(TEXT("NAMC"));

	NAInputBufferComp = CreateDefaultSubobject<UInputBufferComponent>(TEXT("NormalAttacksInputBufferComp"));
}

void UNormalComboAttackComponent::BeginPlay()
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

		if (GetOwner()->HasAuthority() && NAttackAbility4)
		{
			//プレイヤーにNAttackAbility2付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbility4, 1));
		}

		if (GetOwner()->HasAuthority() && NAttackAbilityFinish)
		{
			//プレイヤーにNAttackAbility3付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbilityFinish, 1));
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
void UNormalComboAttackComponent::ExecuteNormalAttackAbility()
{
	if (NAInputBufferComp)
	{
		NAInputBufferComp->KeepOrExeFunction([this]()
			{
				ExectueNormalAttackAbility2();
			});
	}

}

void UNormalComboAttackComponent::ExectueNormalAttackAbility2()
{
	//タグ
	//入力が可能かどうかのタグ
	FGameplayTag PossibleNAttack = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.CantAttack"));

	//三段攻撃派生タグ
	FGameplayTag NextTag0 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag0"));
	FGameplayTag NextTag1 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag1"));
	FGameplayTag NextTag2 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag2"));
	FGameplayTag NextTag3 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag3"));
	FGameplayTag NextTag4 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag4"));

	//金太郎タグ
	FGameplayTag kintaroTag = FGameplayTag::RequestGameplayTag(TEXT("State.Skill.Kintaro"));

	FGameplayTag SkillAttack = FGameplayTag::RequestGameplayTag(TEXT("State.Attacking.Skill"));
	//


	if (!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack) || AbilitySystemComponent->HasMatchingGameplayTag(SkillAttack))
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
				//アビリティ実行
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
		if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag4))
		{
			//アビリティがあるなら
			if (NAttackAbilityFinish)
			{
				//アビリティ実行as
				AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbilityFinish);
				AbilitySystemComponent->RemoveLooseGameplayTag(NextTag4);
			}
		}

		//現在のコンボ進行タグが2なら
		else if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag3))
		{
			//アビリティがあるなら
			if (NAttackAbility4)
			{
				//アビリティ実行as
				AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility4);
				AbilitySystemComponent->RemoveLooseGameplayTag(NextTag3);
			}
		}

		//現在のコンボ進行タグが2なら
		else if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag2))
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

		//else if(AbilitySystemComponent->HasMatchingGameplayTag(NextTag0))
		//{
		//	AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility1);
		//	AbilitySystemComponent->RemoveLooseGameplayTag(NextTag0);
		//}

		//入力が可能だったら
		else if (!AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack))
		{
			AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility1);
		}
		MCC->StartWarping(600.f);

	}
}


