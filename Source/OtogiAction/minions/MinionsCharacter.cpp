#include "MinionsCharacter.h"
#include "minionsAttackComponent.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "Ability/GA_minionsAttack_Normal.h"
#include "Ability/GA_minionsAttack_Middle.h"
#include "Ability/GA_minionsAttack_Strong.h"
#include "AIController.h"
#include "BrainComponent.h"

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
	
	if (StatusComponent)
	{
		StatusComponent->OnDead.AddDynamic(this, &AMinionsCharacter::Dead);
	}

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
	if (HasAuthority())
	{
		int32 InputID = 0;

		// ブループリント側で設定されたアビリティをループで全て付与
		for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
		{
			if (AbilityClass)
			{
				AbilitySystemComponent->GiveAbility(
					FGameplayAbilitySpec(AbilityClass, 1, InputID)
				);

				UE_LOG(LogTemp, Warning, TEXT("Ability Granted: %s"), *AbilityClass->GetName());
				InputID++;
			}
		}
	}
}

//死
void AMinionsCharacter::Dead()
{
	UE_LOG(LogTemp, Warning, TEXT("minions dead"));

	//AI停止
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->BrainComponent->StopLogic(TEXT("Dead"));
	}

	//コリジョン停止
	SetActorEnableCollision(false);
}


void AMinionsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

