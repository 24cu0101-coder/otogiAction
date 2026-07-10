// Fill out your copyright notice in the Description page of Project Settings.


#include "StrongAttackComponent.h"
#include "GameplayTagContainer.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"


UStrongAttackComponent::UStrongAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UStrongAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	PlayerActor = Cast<APlayerCharacter>(OwnerActor);
	SAttackASC = PlayerActor->GetAbilitySystemComponent();

	if(SAttackAbility)
	{
		if (GetOwner()->HasAuthority() && SAttackAbility)
		{
			//プレイヤーにSAttackAbilityを付与する
			DodgeHandle = SAttackASC->GiveAbility(FGameplayAbilitySpec(SAttackAbility, 1));
		}
	
	}
	else
	{
		return;
	}

}


// Called every frame
void UStrongAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//通常攻撃アビリティ起動関数
void UStrongAttackComponent::ExecuteStrongAttackAbility()
{

	FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge"));

	FGameplayTag SAttackTag = FGameplayTag::RequestGameplayTag(FName("PlayerNotify.CantAttack"));

	//アビリティシステムコンポーネントがあり、回避と攻撃が実装中じゃなければ
	if (SAttackASC->HasMatchingGameplayTag(DodgeTag) || SAttackASC->HasMatchingGameplayTag(SAttackTag))
	{
		return;
	}

	else
	{
		//回避アビリティがあるなら
		if (SAttackAbility)
		{
			//AbilitySystemComponent->RemoveLooseGameplayTag(NAttackTag);

			//アビリティ実行
			SAttackASC->TryActivateAbilityByClass(SAttackAbility);
		}
	}

}
