#include "minionsAttackComponent.h"
#include "MinionsCharacter.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "Ability/GA_minionsAttack_Normal.h"

UminionsAttackComponent::UminionsAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UminionsAttackComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UminionsAttackComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bShowDebug)
	{
		return;
	}

	DebugTimer += DeltaTime;

	// 0.2秒ごとに更新（軽量化）
	if (DebugTimer < 0.1f)
	{
		return;
	}

	DebugTimer = 0.f;

	DrawAttackRange();
}

void UminionsAttackComponent::DrawAttackRange()
{
	DrawDebugCircle(
		GetWorld(),
		GetOwner()->GetActorLocation(),
		AttackRange,
		32,
		FColor::Blue,
		false,
		0.2f,
		0,
		2.0f,
		FVector(1, 0, 0),
		FVector(0, 1, 0),
		false
	);
}

APlayerCharacter* UminionsAttackComponent::GetPlayer() const
{
	return Cast<APlayerCharacter>(
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

UStatusComponent* UminionsAttackComponent::GetStatusComponent() const
{
	return GetOwner()->FindComponentByClass<UStatusComponent>();
}

UAbilitySystemComponent* UminionsAttackComponent::GetASC() const
{
	AMinionsCharacter* OwnerEnemy = Cast<AMinionsCharacter>(GetOwner());
	if (!OwnerEnemy)
	{
		return nullptr;
	}

	return OwnerEnemy->GetAbilitySystemComponent();
}
bool UminionsAttackComponent::CanAttack() const
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!Player)
	{
		return false;
	}

	const float Distance =
		FVector::Distance(
			GetOwner()->GetActorLocation(),
			Player->GetActorLocation());

	return Distance <= AttackRange;
}

void UminionsAttackComponent::Attack()
{
	if (!CanAttack())
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetASC();

	if (!ASC)
	{
		return;
	}

	switch (AttackType)
	{
	case EMinionsAttackType::Normal:
		ASC->TryActivateAbilityByClass(UGA_minionsAttack_Normal::StaticClass());
		break;

	case EMinionsAttackType::middle:
		// 後で追加
		break;

	case EMinionsAttackType::Strong:
		// 後で追加
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Minions Attack Triggered"));
}