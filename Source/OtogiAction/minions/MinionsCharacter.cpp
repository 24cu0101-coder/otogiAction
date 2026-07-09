#include "MinionsCharacter.h"
#include "minionsAttackComponent.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "Ability/GA_minionsAttack_Normal.h"

AMinionsCharacter::AMinionsCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//ステータス
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	//攻撃コンポーネント
	AttackComponent = CreateDefaultSubobject<UminionsAttackComponent>(TEXT("AttackComponent")); 
	
	// GASコンポーネント生成
	AbilitySystemComponent =
		CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

}

UAbilitySystemComponent* AMinionsCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMinionsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is NULL"));
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	GiveDefaultAbilities();
}

void AMinionsCharacter::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// 通常攻撃を付与
	AbilitySystemComponent->GiveAbility(
		FGameplayAbilitySpec(UGA_minionsAttack_Normal::StaticClass(), 1, 0)
	);

	UE_LOG(LogTemp, Warning, TEXT("GAS Ability Granted"));
}

void AMinionsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

