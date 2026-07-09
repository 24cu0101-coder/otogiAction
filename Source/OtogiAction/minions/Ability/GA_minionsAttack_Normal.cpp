#include "GA_minionsAttack_Normal.h"
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

	bool bHit = GetWorld()->OverlapMultiByChannel(
		Results,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		Sphere
	);

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
			APlayerCharacter* Player = Cast<APlayerCharacter>(Hit.GetActor());

			if (Player)
			{
				UE_LOG(LogTemp, Warning, TEXT("HIT DETECTED"));
				if (UStatusComponent* Status = Player->FindComponentByClass<UStatusComponent>())
				{
					Status->TakeDamage(Damage);

					UE_LOG(LogTemp, Warning, TEXT("HIT! Player Damaged"));
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	
}