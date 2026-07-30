// Fill out your copyright notice in the Description page of Project Settings.


#include "IaiAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "../Move/MoveComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/InputBuffer/InputBufferComponent.h"


UIaiAttackComponent::UIaiAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MCC = CreateDefaultSubobject<UMoveComponent>(TEXT("SAMC"));

	IaiAttackInputBufferComp = CreateDefaultSubobject<UInputBufferComponent>(TEXT("IaiAttacksInputBufferComp"));

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
	if (IaiAttackInputBufferComp)
	{
		IaiAttackInputBufferComp->KeepOrExeFunction([this]()
			{
				ExecuteIaiAttackAbility2();
			});
	}

}


void UIaiAttackComponent::ExecuteIaiAttackAbility2()
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
			//AbilitySystemComponent->RemoveLooseGameplayTag(NAttackTag);
			//アビリティ実行
			IaiAttackASC->TryActivateAbilityByClass(IaiAttackAbility);
		}
	}
}
