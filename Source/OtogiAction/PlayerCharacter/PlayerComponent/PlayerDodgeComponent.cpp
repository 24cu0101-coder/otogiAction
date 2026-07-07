
//--------------------------------------------------------------------------------------------------------
//回避コンポーネント
//-----------------------------------------------------------------------------------------------------
#include "PlayerDodgeComponent.h"
#include "GameplayTagContainer.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"




UPlayerDodgeComponent::UPlayerDodgeComponent()
{
	

	PrimaryComponentTick.bCanEverTick = true;

		//CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("DodgeComponentASC"));
	
}


// Called when the game starts
void UPlayerDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	PlayerActor = Cast<APlayerCharacter>(OwnerActor);


	AbilitySystemComponent = PlayerActor->GetAbilitySystemComponent();


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
	FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge"));

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
}

