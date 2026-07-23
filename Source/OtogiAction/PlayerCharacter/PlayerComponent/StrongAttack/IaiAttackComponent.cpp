// Fill out your copyright notice in the Description page of Project Settings.


#include "IaiAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "../Move/MoveComponent.h"

UIaiAttackComponent::UIaiAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MCC = CreateDefaultSubobject<UMoveComponent>(TEXT("SAMC"));
}


// Called when the game starts
void UIaiAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	PlayerActor = Cast<APlayerCharacter>(OwnerActor);
	IaiAttackASC = PlayerActor->GetAbilitySystemComponent();

	//強攻撃のアビリティがあれば
	if (IaiAttackAbility)
	{
		if (GetOwner()->HasAuthority() && IaiAttackAbility)
		{
			//プレイヤーにSAttackAbilityを付与する
			IaiHandle = IaiAttackASC->GiveAbility(FGameplayAbilitySpec(IaiAttackAbility, 1));
		}

	}
	else
	{

		//なければリターン
		return;
	}

}


void UIaiAttackComponent::ExecuteIaiAttackAbility()
{
	FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge"));

	FGameplayTag SAttackTag = FGameplayTag::RequestGameplayTag(FName("PlayerNotify.CantAttack"));

	//アビリティシステムコンポーネントがあり、回避と攻撃が実装中じゃなければ
	if (IaiAttackASC->HasMatchingGameplayTag(DodgeTag) || IaiAttackASC->HasMatchingGameplayTag(SAttackTag))
	{
		return;
	}
	else
	{
		//強攻撃アビリティがあるなら
		if (IaiAttackAbility)
		{
			MCC->StartWarping(600.f);
			//AbilitySystemComponent->RemoveLooseGameplayTag(NAttackTag);
			//アビリティ実行
			IaiAttackASC->TryActivateAbilityByClass(IaiAttackAbility);
		}
	}
}
