#include "PlayerDodgeComponent.h"

UPlayerDodgeComponent::UPlayerDodgeComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("DodgeComponentASC"));
	
}


// Called when the game starts
void UPlayerDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(GetOwner(), GetOwner());
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



void UPlayerDodgeComponent::ExecuteAbility()
{

	
	UE_LOG(LogTemp, Log, TEXT("oo"));

	//アビリティシステムコンポーネントがあるなら
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("yy"));

		//回避アビリティがあるなら
		if (DodgeAbility)
		{
			AbilitySystemComponent->TryActivateAbilityByClass(DodgeAbility);
		}
	}

	else
	{
		UE_LOG(LogTemp, Log, TEXT("jj"));

	}

}

