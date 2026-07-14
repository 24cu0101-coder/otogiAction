#include "MinionsCharacter.h"
#include "minionsAttackComponent.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "Ability/GA_minionsAttack_Normal.h"
#include "Ability/GA_minionsAttack_Middle.h"
#include "Ability/GA_minionsAttack_Strong.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "../Orb/OrbActor.h"

AMinionsCharacter::AMinionsCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//ステータス
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	//攻撃コンポーネント
	AttackComponent = CreateDefaultSubobject<UminionsAttackComponent>(TEXT("AttackComponent"));

	// GASコンポーネント生成
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	//オーブスポーンコンポーネント
	OrbSpawnComponent = CreateDefaultSubobject<UOrbSpawnComponent>(TEXT("OrbSpawnComponent"));
}

UAbilitySystemComponent* AMinionsCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMinionsCharacter::BeginPlay()
{
	Super::BeginPlay();

	//ダメージを受けたらOnDamageを呼ぶ
	OnTakeAnyDamage.AddDynamic(this, &AMinionsCharacter::OnDamage);

	if (StatusComponent)
	{
		StatusComponent->OnDead.AddDynamic(this, &AMinionsCharacter::Dead);
	}

	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is NULL"));
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	GiveDefaultAbilities();

	AttackComponent->Attack();
}

void AMinionsCharacter::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	if (HasAuthority())
	{
		int32 InputID = 0;

		// ブループリント側で設定されたアビリティをループで全て付与
		for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
		{
			if (AbilityClass)
			{
				AbilitySystemComponent->GiveAbility(
					FGameplayAbilitySpec(AbilityClass, 1, InputID)
				);

				UE_LOG(LogTemp, Warning, TEXT("Ability Granted: %s"), *AbilityClass->GetName());
				InputID++;
			}
		}
	}
}

void AMinionsCharacter::OnDamage(
	AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (!StatusComponent || !OrbSpawnComponent)
	{
		return;
	}

	// ダメージを受ける前の現在HP
	float CurrentHP = StatusComponent->GetCurrentHP();

	// 実際に減るHP以上のOrbを出さない
	float ActualDamage = FMath::Min(Damage, CurrentHP);

	// 10ダメージにつきOrb1個
	int32 OrbCount = FMath::FloorToInt(ActualDamage / 10.f);

	// 10未満のダメージでも最低1個出す
	if (OrbCount <= 0)
	{
		OrbCount = 1;
	}


	UE_LOG(LogTemp, Warning,
		TEXT("Minion Damage:%f CurrentHP:%f SpawnOrb:%d"),
		Damage,
		CurrentHP,
		OrbCount);


	for (int32 i = 0; i < OrbCount; i++)
	{

		UE_LOG(LogTemp, Warning,
			TEXT("Spawn Orb %d"),
			i);

		AOrbActor* Orb = OrbSpawnComponent->SpawnOrb();

		if (Orb)
		{
			// このMinionが出したOrbとして登録
			Orb->SetOwnerEnemy(this);
		}
	}
}
/*void AMinionsCharacter::OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	AOrbActor* SpawnedOrb = OrbSpawnComponent->SpawnOrb();

	if (OrbSpawnComponent)
	{
		AOrbActor* Orb = OrbSpawnComponent->SpawnOrb();

		if (Orb)
		{
			Orb->SetOwnerEnemy(this);
		}
	}
}*/

//死
void AMinionsCharacter::Dead()
{
	UE_LOG(LogTemp, Warning, TEXT("minions dead"));

	//AI停止
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->BrainComponent->StopLogic(TEXT("Dead"));
	}

	//コリジョン停止
	SetActorEnableCollision(false);


	//いったん仮でDestroy
	Destroy();
}


void AMinionsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

