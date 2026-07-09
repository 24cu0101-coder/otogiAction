
//--------------------------------------------------------------------------------------------------------
//通常攻撃コンポーネント
//-----------------------------------------------------------------------------------------------------
#include "NormalAttackComponent.h"
#include "GameplayTagContainer.h"


UNormalAttackComponent::UNormalAttackComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("NAttackComponentASC"));

}


//
void UNormalAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(GetOwner(), GetOwner());
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

	}	
}


// Called every frame
void UNormalAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNormalAttackComponent::ExecuteNormalAttackAbility()
{
	//タグ
	//入力が可能かどうかのタグ
	FGameplayTag PossibleNAttack = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.CantAttack"));

	//三段攻撃派生タグ
	FGameplayTag NextTag1 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag1"));
	FGameplayTag NextTag2 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag2"));


	//現在のコンボ進行タグが2なら
	if(AbilitySystemComponent->HasMatchingGameplayTag(NextTag2))
	{
		//アビリティがあるなら
		if (NAttackAbility3)
		{
			//アビリティ実行
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
	else if(!AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack))
	{
		AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility1);
	}

	if (!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack))
	{

		return;
	}
}

