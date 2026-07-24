#include "minionsAttackComponent.h"
#include "MinionsCharacter.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "Ability/GA_minionsAttack_Normal.h"
#include "Ability/GA_minionsAttack_Middle.h"
#include "Ability/GA_minionsAttack_Strong.h"

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


	UE_LOG(LogTemp, Warning,
		TEXT("Distance = %.1f  AttackRange = %.1f"),
		Distance,
		AttackRange);

	return Distance <= AttackRange;
}

/*void UminionsAttackComponent::Attack()
{
	APlayerCharacter* Player = GetPlayer();

	if (!Player)
	{
		return;
	}
	---------------------------------------------
	float Distance = FVector::Distance(
		GetOwner()->GetActorLocation(),
		Player->GetActorLocation());
	 距離で攻撃を決定
	if (Distance <= 200.f)
	{
		AttackType = EMinionsAttackType::Normal;
	}
	else if (Distance <= 500.f)
	{
		AttackType = EMinionsAttackType::middle;
	}
	else if (Distance<=800)
	{
		AttackType = EMinionsAttackType::Strong;
	}
	---------------------------------------------

	int32 RandomAttack = FMath::RandRange(0, 99);

	if (RandomAttack < 60)
	{
		AttackType = EMinionsAttackType::Normal;
	}
	else if (RandomAttack < 90)
	{
		AttackType = EMinionsAttackType::middle;
	}
	else
	{
		AttackType = EMinionsAttackType::Strong;
	}

	UE_LOG(LogTemp, Warning, TEXT("Attack() Called"));

	if (!CanAttack())
	{
		UE_LOG(LogTemp, Error, TEXT("CanAttack = FALSE"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("CanAttack = TRUE"));

	UAbilitySystemComponent* ASC = GetASC();

	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is NULL"));
		return;
	}

	switch (AttackType)
	{
	case EMinionsAttackType::Normal:
		UE_LOG(LogTemp, Warning, TEXT("Normal Attack"));
		ASC->TryActivateAbilityByClass(UGA_minionsAttack_Normal::StaticClass());
		break;

	case EMinionsAttackType::middle:
		UE_LOG(LogTemp, Warning, TEXT("Middle Attack"));
		ASC->TryActivateAbilityByClass(UGA_minionsAttack_Middle::StaticClass());
		break;

	case EMinionsAttackType::Strong:
		UE_LOG(LogTemp, Warning, TEXT("Strong Attack"));
		ASC->TryActivateAbilityByClass(UGA_minionsAttack_Strong::StaticClass());
		break;
	}
}*/
void UminionsAttackComponent::Attack()
{
	APlayerCharacter* Player = GetPlayer();
	if (!Player)
	{
		return;
	}

	// らんダムで攻撃タイプの決定 (Normal: 60%, Middle: 30%, Strong: 10%)
	int32 RandomAttack = FMath::RandRange(0, 99);
	if (RandomAttack < 60)
	{
		AttackType = EMinionsAttackType::Normal;
	}
	else if (RandomAttack < 90)
	{
		AttackType = EMinionsAttackType::middle;
	}
	else
	{
		AttackType = EMinionsAttackType::Strong;
	}

	UE_LOG(LogTemp, Warning, TEXT("Attack() Called"));

	if (!CanAttack())
	{
		UE_LOG(LogTemp, Error, TEXT("CanAttack = FALSE"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("CanAttack = TRUE"));

	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is NULL"));
		return;
	}

	// 発動したいアビリティのタグを入れる変数
	FGameplayTagContainer TargetTagContainer;

	switch (AttackType)
	{
	case EMinionsAttackType::Normal:
		UE_LOG(LogTemp, Warning, TEXT("Normal Attack"));
		TargetTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Normal")));
		break;

	case EMinionsAttackType::middle:
		UE_LOG(LogTemp, Warning, TEXT("Middle Attack"));
		TargetTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Middle")));
		break;

	case EMinionsAttackType::Strong:
		UE_LOG(LogTemp, Warning, TEXT("Strong Attack"));
		TargetTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Strong")));
		break;
	}

	// タグを使ってアビリティを発動
	if (TargetTagContainer.IsValid())
	{
		ASC->TryActivateAbilitiesByTag(TargetTagContainer);
	}
}
void UminionsAttackComponent::ExecuteAttackHit()
{
	AActor* OwnerActor = GetOwner();

	if (!OwnerActor)
	{
		return;
	}


	FVector Center =
		OwnerActor->GetActorLocation();


	float Radius = 200.f;


	DrawDebugSphere(
		GetWorld(),
		Center,
		Radius,
		24,
		FColor::Red,
		false,
		1.f);


	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerActor);


	TArray<AActor*> OutActors;


	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		Radius,
		{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
		APlayerCharacter::StaticClass(),
		IgnoreActors,
		OutActors);



	for (AActor* Actor : OutActors)
	{
		APlayerCharacter* Player =
			Cast<APlayerCharacter>(Actor);


		if (Player)
		{
			UGameplayStatics::ApplyDamage(
				Player,
				20.f,
				OwnerActor->GetInstigatorController(),
				OwnerActor,
				UDamageType::StaticClass());


			UE_LOG(LogTemp, Warning, TEXT("Notify Damage"));
		}
	}
}