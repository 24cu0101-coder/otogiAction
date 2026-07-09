
//--------------------------------------------------------------------------------------------------------
//回避コンポーネント
//-----------------------------------------------------------------------------------------------------
#include "PlayerDodgeComponent.h"
#include "GameplayTagContainer.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"




UPlayerDodgeComponent::UPlayerDodgeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	
}


// Called when the game starts
void UPlayerDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	//
	AActor* OwnerActor = GetOwner();
	PlayerActor = Cast<APlayerCharacter>(OwnerActor);
	AbilitySystemComponent = PlayerActor->GetAbilitySystemComponent();


	//AbilitySystemComponent->InitAbilityActorInfo(GetOwner(), GetOwner());
	{
		if (GetOwner()->HasAuthority() && DodgeAbility)
		{
			//プレイヤーにDodgeAbilityを付与する
			DodgeHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DodgeAbility, 1));
		}
	}
	
	
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
	////ヌルチェック
	//if (!DodgeASC || !DodgeAbility)
	//{
	//	return;
	//}
	//if (GetOwner()->HasAuthority())
	//{
	//	//プレイヤーにDodgeAbilityを付与する
	//	DodgeHandle = DodgeASC->GiveAbility(FGameplayAbilitySpec(DodgeAbility, 1));
	//}
}

void UPlayerDodgeComponent::ExecuteAbility()
{
	FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge"));

	FGameplayTag NAttackTag = FGameplayTag::RequestGameplayTag(FName("PlayerNotify.CantAttack"));

	//アビリティシステムコンポーネントがあり、回避と攻撃が実装中じゃなければ
	if (AbilitySystemComponent->HasMatchingGameplayTag(DodgeTag) || AbilitySystemComponent->HasMatchingGameplayTag(NAttackTag))
	{
		return;
	}

	else 
	{
		//回避アビリティがあるなら
		if (DodgeAbility)
		{
			//AbilitySystemComponent->RemoveLooseGameplayTag(NAttackTag);

			//アビリティ実行
			AbilitySystemComponent->TryActivateAbilityByClass(DodgeAbility);
		}
	}
}

