
//--------------------------------------------------------------------------------------------------------
//回避コンポーネント
//-----------------------------------------------------------------------------------------------------
#include "PlayerDodgeComponent.h"
#include "GameplayTagContainer.h"



UPlayerDodgeComponent::UPlayerDodgeComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	//アビリティシステムコンポーネントがあるなら
	if (AbilitySystemComponent)
	{
		//回避アビリティがあるなら
		if (DodgeAbility)
		{

		}

	}
}


// Called when the game starts
void UPlayerDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UPlayerDodgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//
void UPlayerDodgeComponent::DodgeAbilitySet(UAbilitySystemComponent* DodgeASC)
{
	//ヌルチェック
	if (!DodgeASC || !DodgeAbility)
	{
		return;
	}

	if (GetOwner()->HasAuthority())
	{
		//プレイヤーにDodgeAbilityを付与する
		DodgeHandle = DodgeASC->GiveAbility(FGameplayAbilitySpec(DodgeAbility, 1));
	}
}

void UPlayerDodgeComponent::ExecuteAbility()
{
	FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge"));

<<<<<<< HEAD
=======
	FGameplayTag NAttackTag = FGameplayTag::RequestGameplayTag(FName("PlayerNotify.CantAttack"));

	//アビリティシステムコンポーネントがあり、回避と攻撃が実装中じゃなければ
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!AbilitySystemComponent->HasMatchingGameplayTag(DodgeTag) && !AbilitySystemComponent->HasMatchingGameplayTag(NAttackTag))
	{
		//回避アビリティがあるなら
		if (DodgeAbility)
		{
			//アビリティ実行
			AbilitySystemComponent->TryActivateAbilityByClass(DodgeAbility);
		}
	}
>>>>>>> origin/master
}

