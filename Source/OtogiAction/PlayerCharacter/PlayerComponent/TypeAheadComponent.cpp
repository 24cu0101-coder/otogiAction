// Fill out your copyright notice in the Description page of Project Settings.


#include "TypeAheadComponent.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/NormalAttack/NormalComboAttackComponent.h"


UTypeAheadComponent::UTypeAheadComponent()
{
	PrimaryComponentTick.bCanEverTick = true;



}


// Called when the game starts
void UTypeAheadComponent::BeginPlay()
{
	Super::BeginPlay();

	//AActor* OwnerActor = GetOwner();
	//PlayerActor = Cast<APlayerCharacter>(OwnerActor);
	//AbilitySystemComponent = PlayerActor->GetAbilitySystemComponent();

	if (UAbilitySystemComponent* Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		FGameplayTag TypeAheadTag = FGameplayTag::RequestGameplayTag(TEXT("TypeAheadReception"));
		Asc->GenericGameplayEventCallbacks.FindOrAdd(TypeAheadTag).AddUObject(this, &UTypeAheadComponent::vss);

	}

}

//
bool UTypeAheadComponent::TypeAhead()
{
	//先行入力を受け付けるタグ　
	FGameplayTag TypeAheadTag = FGameplayTag::RequestGameplayTag(TEXT("TypeAheadReception"));

	if (AbilitySystemComponent->HasMatchingGameplayTag(TypeAheadTag))
	{
		return true;

	}

	return false;

}




// Called every frame
void UTypeAheadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

}

void UTypeAheadComponent::GetBufferedAction(TFunction<void()> Function)
{
	ExeFunction = Function;
}

TFunction<void()> UTypeAheadComponent::SetBufferedAction()
{
	return ExeFunction;
}

void UTypeAheadComponent::vss(const FGameplayEventData* Payload)
{
	//BufferedAction = 
}