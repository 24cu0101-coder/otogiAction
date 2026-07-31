#include "minionsAttackComponent.h"
#include "AIController.h"
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
	Super::TickComponent(
		DeltaTime,
		TickType,
		ThisTickFunction);

	//--------------------------------
	// AttackRange内ならPlayerを見る
	//--------------------------------

	APawn* Pawn = Cast<APawn>(GetOwner());

	if (Pawn)
	{
		AAIController* AI =
			Cast<AAIController>(Pawn->GetController());

		APlayerCharacter* Player = GetPlayer();

		if (AI && Player)
		{
			if (CanAttack())
			{
				AI->SetFocus(Player);
			}
			else
			{
				AI->ClearFocus(EAIFocusPriority::Gameplay);
			}
		}
	}

	//--------------------------------
	// デバッグ表示
	//--------------------------------

	if (!bShowDebug)
	{
		return;
	}

	DebugTimer += DeltaTime;

	//0.1秒ごとに更新
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

void UminionsAttackComponent::Attack()
{
	AMinionsCharacter* Minion =
		Cast<AMinionsCharacter>(GetOwner());


	APlayerCharacter* Player = GetPlayer();

	if (!Player)
	{
		return;
	}


	UE_LOG(LogTemp, Warning,
		TEXT("Attack() Called"));


	// 攻撃距離チェック
	if (!CanAttack())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("CanAttack = FALSE"));

		if (Minion)
		{
			Minion->SetIsAttacking(false);
		}

		return;
	}


	UAbilitySystemComponent* ASC = GetASC();

	if (!ASC)
	{
		UE_LOG(LogTemp, Error,
			TEXT("ASC is NULL"));

		if (Minion)
		{
			Minion->SetIsAttacking(false);
		}

		return;
	}



	//=========================
	// 攻撃タイプ抽選
	//=========================

	int32 RandomAttack =
		FMath::RandRange(0, 99);


	if (RandomAttack < 60)
	{
		AttackType =
			EMinionsAttackType::Normal;

		UE_LOG(LogTemp, Warning,
			TEXT("Normal Attack"));
	}
	else if (RandomAttack < 90)
	{
		AttackType =
			EMinionsAttackType::middle;

		UE_LOG(LogTemp, Warning,
			TEXT("Middle Attack"));
	}
	else
	{
		AttackType =
			EMinionsAttackType::Strong;

		UE_LOG(LogTemp, Warning,
			TEXT("Strong Attack"));
	}



	//=========================
	// GameplayTag作成
	//=========================

	FGameplayTagContainer TargetTagContainer;


	switch (AttackType)
	{

	case EMinionsAttackType::Normal:

		TargetTagContainer.AddTag(
			FGameplayTag::RequestGameplayTag(
				FName("Ability.Attack.Normal")));

		break;



	case EMinionsAttackType::middle:

		TargetTagContainer.AddTag(
			FGameplayTag::RequestGameplayTag(
				FName("Ability.Attack.Middle")));

		break;



	case EMinionsAttackType::Strong:

		TargetTagContainer.AddTag(
			FGameplayTag::RequestGameplayTag(
				FName("Ability.Attack.Strong")));

		break;

	}



	//=========================
	// GAS発動
	//=========================

	bool bActivated =
		ASC->TryActivateAbilitiesByTag(
			TargetTagContainer);



	//=========================
	// 攻撃中フラグ
	//=========================

	if (Minion)
	{

		if (bActivated)
		{
			Minion->SetIsAttacking(true);

			UE_LOG(LogTemp, Warning,
				TEXT("Minion Attack Activated"));
		}
		else
		{
			Minion->SetIsAttacking(false);

			UE_LOG(LogTemp, Warning,
				TEXT("Minion Attack Failed"));
		}

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
		UE_LOG(LogTemp, Warning,
			TEXT("Hit Actor = %s"),
			*Actor->GetName());

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
void UminionsAttackComponent::SetShowDebug(bool bEnable)
{
	bShowDebug = bEnable;
}