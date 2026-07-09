#include "GA_minionsAttack_Normal.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "AbilitySystemComponent.h"

#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/Component/Status/StatusComponent.h"

#include "DrawDebugHelpers.h"

UGA_minionsAttack_Normal::UGA_minionsAttack_Normal()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_minionsAttack_Normal::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("GA NORMAL START"));
	APlayerCharacter* Player =
		Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Found = %s"), *Player->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player NOT FOUND"));
	}
	AActor* OwnerActor = ActorInfo->AvatarActor.Get();

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner NULL"));

		EndAbility(
			Handle,
			ActorInfo,
			ActivationInfo,
			true,
			false);

		return;
	}

	//---------------------------------------------------
	// 攻撃範囲
	//---------------------------------------------------

	const FVector Center = OwnerActor->GetActorLocation();
	const float Radius = 200.f;

	DrawDebugSphere(
		GetWorld(),
		Center,
		Radius,
		24,
		FColor::Red,
		false,
		1.0f,
		0,
		2.0f);

	//---------------------------------------------------
	// Player検索
	//---------------------------------------------------

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerActor);

	TArray<AActor*> OutActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(
		UEngineTypes::ConvertToObjectType(ECC_Pawn));

	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Center,
		Radius,
		ObjectTypes,
		APlayerCharacter::StaticClass(),
		IgnoreActors,
		OutActors);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Overlap = %s"),
		bHit ? TEXT("TRUE") : TEXT("FALSE"));

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Hit Count = %d"),
		OutActors.Num());

	//---------------------------------------------------
	// Playerに当たった
	//---------------------------------------------------

	for (AActor* Actor : OutActors)
	{
		if (!Actor)
		{
			continue;
		}

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Hit Actor = %s"),
			*Actor->GetName());
		Player = Cast<APlayerCharacter>(Actor);

		if (!Player)
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("HIT DETECTED"));

		// PlayerがStatusComponentを持っていたらダメージ
		if (UStatusComponent* Status =
			Player->FindComponentByClass<UStatusComponent>())
		{
			Status->TakeDamage(Damage);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Player Damaged"));
		}
	}

	EndAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		true,
		false);
}

/*#include "GA_minionsAttack_Normal.h"
#include "Kismet/GamePlayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "DrawDebugHelpers.h"

UGA_minionsAttack_Normal::UGA_minionsAttack_Normal()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_minionsAttack_Normal::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("GA START"));
	AActor* OwnerActor = ActorInfo->AvatarActor.Get();
	if (!OwnerActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	// 攻撃位置
	FVector Center = OwnerActor->GetActorLocation();

	float Radius = 200.f; 

	// 当たり判定
	TArray<FOverlapResult> Results;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	UE_LOG(LogTemp, Warning, TEXT("GA START"));



	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner NULL"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Owner OK"));

	bool bHit = GetWorld()->OverlapMultiByChannel(
		Results,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		Sphere
	);
	UE_LOG(LogTemp, Warning, TEXT("Overlap = %s"), bHit ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(LogTemp, Warning, TEXT("Hit Count = %d"), Results.Num());
	// デバッグ表示
	DrawDebugSphere(
		GetWorld(),
		Center,
		Radius,
		24,
		FColor::Red,
		false,
		1.0f
	);

	if (bHit)
	{
		for (auto& Hit : Results)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loop"));
			AActor* Actor = Hit.GetActor();

			if (Actor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor = %s"), *Actor->GetName());
				UE_LOG(LogTemp, Warning, TEXT("Class = %s"), *Actor->GetClass()->GetName());
			}

			APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);

			if (Player)
			{
				UE_LOG(LogTemp, Warning, TEXT("HIT DETECTED"));
			}
			/APlayerCharacter* Player = Cast<APlayerCharacter>(Hit.GetActor());

			if (Player)
			{
				UE_LOG(LogTemp, Warning, TEXT("HIT DETECTED"));
				if (UStatusComponent* Status = Player->FindComponentByClass<UStatusComponent>())
				{
					Status->TakeDamage(Damage);

					UE_LOG(LogTemp, Warning, TEXT("HIT! Player Damaged"));
				}
			}/
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
}*/
