// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyAttack01Component.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

// Sets default values for this component's properties
UBossEnemyAttack01Component::UBossEnemyAttack01Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossEnemyAttack01Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* Owner = GetOwner();

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Owner);
	UAbilitySystemComponent* ASC = ASI ? ASI->GetAbilitySystemComponent() : nullptr;

	if (ASC)
	{
		for (TSubclassOf<UGameplayAbility> AbilityClass : AttackAbility)
		{
			if (AbilityClass)
			{
				 //GiveAbilityにFGameplayAbilitySpecを渡して付与
				 //引数: FGameplayAbilitySpec(クラス, レベル, 入力ID)
				ASC->GiveAbility(FGameplayAbilitySpec(GetClass(), 1, INDEX_NONE));
			}
		}
	}
}


// Called every frame
void UBossEnemyAttack01Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

