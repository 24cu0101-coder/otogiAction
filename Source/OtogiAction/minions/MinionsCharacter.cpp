#include "MinionsCharacter.h"
#include "minionsAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "Ability/GA_minionsAttack_Normal.h"
#include "Ability/GA_minionsAttack_Middle.h"
#include "Ability/GA_minionsAttack_Strong.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "../Orb/OrbActor.h"
#include "Components/WidgetComponent.h"
#include "../UI/EnemyHPWidget.h"
#include "OtogiAction/Component/Collision/SphereCollisionComponent.h"
#include "OtogiAction/minions/MinionsHitReactionComponent.h"


AMinionsCharacter::AMinionsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//ステータス
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	//攻撃コンポーネント
	AttackComponent = CreateDefaultSubobject<UminionsAttackComponent>(TEXT("AttackComponent"));

	// GASコンポーネント生成
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	//オーブスポーンコンポーネント
	OrbSpawnComponent = CreateDefaultSubobject<UOrbSpawnComponent>(TEXT("OrbSpawnComponent"));

	//Audioコンポーネント
	CharacterAudioComponent =CreateDefaultSubobject<UCharacterAudioComponent>(TEXT("CharacterAudioComponent"));

	//SphereCollisonComponent
	SphereCollisionComponent =CreateDefaultSubobject<USphereCollisionComponent>(TEXT("SphereCollisionComponent"));

	//HitReaction
	HitReactionComponent = CreateDefaultSubobject<UMinionsHitReactionComponent>(TEXT("HitReactionComponent"));

	//HPwidget
	HPWidgetComponent =CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));

	HPWidgetComponent->SetupAttachment(RootComponent);

	HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
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

		// HP変更時にHPバー更新
		StatusComponent->OnDamaged.AddDynamic(
			this,
			&AMinionsCharacter::UpdateHPWidget);
	}
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is NULL"));
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	GiveDefaultAbilities();

	//AttackComponent->Attack();

	if (UEnemyHPWidget* HPWidget =
		Cast<UEnemyHPWidget>(HPWidgetComponent->GetUserWidgetObject()))
	{
		HPWidget->SetHP(
			StatusComponent->GetCurrentHP(),
			StatusComponent->GetMaxHP());
	}
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

	SetIsHitFlg(true);

	//========================
	// 攻撃中断
	//========================

	if (IsAttacking())
	{
		CancelAttack();
	}

	// 被弾音
	if (CharacterAudioComponent)
	{
		CharacterAudioComponent->PlayCharacterSound(
			ECharacterSoundType::Damage);
	}



	if (!StatusComponent || !OrbSpawnComponent)
	{
		return;
	}



	if (HitReactionComponent && DamageCauser)
	{

		//--------------------------------
		// 攻撃方向保存
		//--------------------------------

		HitReactionComponent->SetHitDirection(
			DamageCauser);



		//--------------------------------
		// 姿勢値を削る
		//--------------------------------

		if (!HitReactionComponent->IsStanceBroken())
		{
			HitReactionComponent->AddStance(Damage);
		}


		//--------------------------------
		// 姿勢崩壊チェック
		//--------------------------------

		if (HitReactionComponent->IsStanceBreak())
		{

			UE_LOG(LogTemp, Warning,
				TEXT("MINION STANCE BREAK"));


			//--------------------------------
			// 大きい怯み
			//--------------------------------

			HitReactionComponent->PlayHitReaction(
				Damage);



			//--------------------------------
			// 姿勢リセット
			//--------------------------------
			HitReactionComponent->SetStanceBroken(true);
			HitReactionComponent->ResetStance();

		}

	}



	//--------------------------------
	// オーブ生成
	//--------------------------------

	OrbSpawnComponent->SpawnOrbs(
		this,
		Damage);

}
//HPWidget
void AMinionsCharacter::UpdateHPWidget(float CurrentHP)
{
	if (!StatusComponent)
	{
		return;
	}

	if (UEnemyHPWidget* HPWidget =
		Cast<UEnemyHPWidget>(HPWidgetComponent->GetUserWidgetObject()))
	{
		HPWidget->SetHP(
			CurrentHP,
			StatusComponent->GetMaxHP());
	}
}

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

	//消えてコリジョンなくす
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (AttackComponent)
	{
		AttackComponent->SetShowDebug(false);
	}
}


void AMinionsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinionsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinionsCharacter::CancelAttack()
{

	bIsAttacking = false;


	// GAS攻撃停止
	if (AbilitySystemComponent)
	{

		FGameplayTagContainer AttackTags;


		AttackTags.AddTag(
			FGameplayTag::RequestGameplayTag(
				FName("Ability.Attack.Normal")));


		AttackTags.AddTag(
			FGameplayTag::RequestGameplayTag(
				FName("Ability.Attack.Middle")));


		AttackTags.AddTag(
			FGameplayTag::RequestGameplayTag(
				FName("Ability.Attack.Strong")));



		AbilitySystemComponent
			->CancelAbilities(
				&AttackTags);

	}


	UE_LOG(LogTemp, Warning,
		TEXT("MINION ATTACK CANCEL"));

}