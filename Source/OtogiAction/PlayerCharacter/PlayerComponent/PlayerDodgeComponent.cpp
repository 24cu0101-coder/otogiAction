#include "PlayerDodgeComponent.h"

UPlayerDodgeComponent::UPlayerDodgeComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
