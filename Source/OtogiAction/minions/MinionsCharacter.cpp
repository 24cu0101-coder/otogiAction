#include "MinionsCharacter.h"
#include "minionsAttackComponent.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "Ability/GA_minionsAttack_Normal.h"
#include "Ability/GA_minionsAttack_Middle.h"
#include "Ability/GA_minionsAttack_Strong.h"

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

	AttackComponent->Attack();
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
	// 中距離攻撃を付与
	AbilitySystemComponent->GiveAbility(
		FGameplayAbilitySpec(UGA_minionsAttack_Middle::StaticClass(), 1, 1)
	);
	// 強攻撃を付与
	AbilitySystemComponent->GiveAbility(
		FGameplayAbilitySpec(UGA_minionsAttack_Strong::StaticClass(), 1, 2)
	);

	UE_LOG(LogTemp, Warning, TEXT("Normal Granted"));
	UE_LOG(LogTemp, Warning, TEXT("Middle Granted"));
	UE_LOG(LogTemp, Warning, TEXT("Strong Granted"));
}

void AMinionsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

