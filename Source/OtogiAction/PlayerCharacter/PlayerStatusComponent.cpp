// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusComponent.h"
#include "PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
}
// Called when the game starts
void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	//現在のHpを最大値にしておく
	CurrentHP = MaxHP;

}
// Called every frame
void UPlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UPlayerStatusComponent::TakeDamage(float Damage)
{
	//playerが無敵状態か死んでたら０を返す
	if (Damage <= 0.f || IsDead() || PlayerInvincible())
	{
		return 0.f;
	}

	//現在のHpから受けたダメージ量減算
	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0.f, MaxHP);

	//HPが変更されたことを通地
	OnDamaged.Broadcast(CurrentHP);

	//デバッグで出力
	UE_LOG(LogTemp, Warning, TEXT("[%s] HP: %f"), *GetOwner()->GetName(), CurrentHP);

	//現在のHPがゼロになったら死亡通知を送る
	if (CurrentHP <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Dead"), *GetOwner()->GetName());
		OnDead.Broadcast();
	}
	return Damage;

}

//HP回復
void UPlayerStatusComponent::Heal(float HealAmount)
{
	//死んでいたら返す
	if (HealAmount <= 0.f || IsDead())return;

	//回復
	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0.f, MaxHP);
	OnDamaged.Broadcast(CurrentHP);

}

//死亡通知
bool UPlayerStatusComponent::IsDead()const
{
	return CurrentHP <= 0.f;
}

//無敵
bool UPlayerStatusComponent::PlayerInvincible()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return false;

	// IAbilitySystemInterface 経由で取得すると、PlayerCharacter 以外の敵・NPCにも対応できて安全です！
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OwnerActor);
	UAbilitySystemComponent* ASC = ASCInterface ? ASCInterface->GetAbilitySystemComponent() : nullptr;

	if (ASC)
	{
		FGameplayTag InvincibleTag = FGameplayTag::RequestGameplayTag(FName("Invincible"));
		if (ASC->HasMatchingGameplayTag(InvincibleTag))
		{
			return true;
		}
	}

	return false;
}

