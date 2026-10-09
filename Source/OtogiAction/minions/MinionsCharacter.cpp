#include "MinionsCharacter.h"
#include "minionsAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
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
#include "NiagaraFunctionLibrary.h"
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
	CharacterAudioComponent = CreateDefaultSubobject<UCharacterAudioComponent>(TEXT("CharacterAudioComponent"));

	//SphereCollisonComponent
	SphereCollisionComponent = CreateDefaultSubobject<USphereCollisionComponent>(TEXT("SphereCollisionComponent"));

	//HitReaction
	HitReactionComponent = CreateDefaultSubobject<UMinionsHitReactionComponent>(TEXT("HitReactionComponent"));

	//HPwidget
	HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));

	HPWidgetComponent->SetupAttachment(RootComponent);

	HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

UAbilitySystemComponent* AMinionsCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMinionsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHPWidgetVisibility();
}

void AMinionsCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Kintaro専用敵はゲーム開始時はダメージ無効
	if (bKintaroOnlyEnemy)
	{
		bCanTakeDamage = false;

		UE_LOG(LogTemp, Warning,
			TEXT("[KINTARO DAMAGE] %s INITIALIZED -> CanTakeDamage = false"),
			*GetName());
	}

	//ダメージを受けたらOnDamageを呼ぶ
	OnTakeAnyDamage.AddDynamic(this, &AMinionsCharacter::OnDamage);

	if (StatusComponent)
	{
		StatusComponent->OnDead.AddDynamic(this, &AMinionsCharacter::Dead);

		// HP変更時にHPバー更新
		StatusComponent->OnDamaged.AddDynamic(this,&AMinionsCharacter::UpdateHPWidget);
	}
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	GiveDefaultAbilities();

	//AttackComponent->Attack();

	if (UEnemyHPWidget* HPWidget =Cast<UEnemyHPWidget>(HPWidgetComponent->GetUserWidgetObject()))
	{
		HPWidget->SetHP(StatusComponent->GetCurrentHP(), StatusComponent->GetMaxHP());
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
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, InputID));

				UE_LOG(LogTemp, Warning, TEXT("Ability Granted: %s"), *AbilityClass->GetName());
				InputID++;
			}
		}
	}
}
void AMinionsCharacter::OnDamage(AActor* DamagedActor,float Damage,const UDamageType* DamageType,AController* InstigatedBy,AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning,
		TEXT("[HIT CHECK] AttackInProgress=%d IsAttacking=%d"),
		bIsAttackInProgress,
		IsAttacking());

	// Kintaro専用敵のダメージ受付判定
	if (bKintaroOnlyEnemy && !bCanTakeDamage)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("%s DAMAGE BLOCKED | KintaroOnly=%d CanTakeDamage=%d"),
			*GetName(),
			bKintaroOnlyEnemy,
			bCanTakeDamage);

		return;
	}

	//NotifyStateで攻撃中か判断
	const bool bIgnoreHitReaction = bIsAttackInProgress;

	//攻撃中でなければ通常の被弾
	if (!bIgnoreHitReaction)
	{
		// ここから通常の被弾処理
		SetIsHitFlg(true);

		// 攻撃中断
		if (IsAttacking())
		{
			CancelAttack();
		}

	}


	// 被弾音
	if (CharacterAudioComponent)
	{
		CharacterAudioComponent->PlayCharacterSound(ECharacterSoundType::Damage);
	}

	// 被弾エフェクト
	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),HitEffect,GetActorLocation() + FVector(0, 0, 80.f));
	}

	if (!StatusComponent)
	{
		return;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("%s DAMAGE ALLOWED | KintaroOnly=%d CanTakeDamage=%d"),
		*GetName(),
		bKintaroOnlyEnemy,
		bCanTakeDamage);

	StatusComponent->TakeDamage(Damage);

	// ヒットリアクション
	if (!bIgnoreHitReaction && HitReactionComponent && DamageCauser)
	{
		HitReactionComponent->SetHitDirection(DamageCauser);

		if (!HitReactionComponent->IsStanceBroken())
		{
			HitReactionComponent->AddStance(Damage);
		}

		if (HitReactionComponent->IsStanceBreak())
		{
			UE_LOG(LogTemp, Warning,
				TEXT("MINION STANCE BREAK"));

			HitReactionComponent->PlayHitReaction(Damage);

			HitReactionComponent->SetStanceBroken(true);
			HitReactionComponent->ResetStance();
		}
	}

	// Orb生成
	if (OrbSpawnComponent)
	{
		if (!bKintaroOnlyEnemy || bCanSpawnOrb)
		{
			OrbSpawnComponent->SpawnOrbs(this, Damage);
		}

		UE_LOG(LogTemp, Warning,
			TEXT("%s KintaroOnly:%d CanSpawn:%d"),
			*GetName(),
			bKintaroOnlyEnemy,
			bCanSpawnOrb);
	}
}

//HPWidget
void AMinionsCharacter::UpdateHPWidget(float CurrentHP)
{
	if (!StatusComponent)
	{
		return;
	}

	if (UEnemyHPWidget* HPWidget = Cast<UEnemyHPWidget>(HPWidgetComponent->GetUserWidgetObject()))
	{
		HPWidget->SetHP(CurrentHP, StatusComponent->GetMaxHP());
	}
}

//死
void AMinionsCharacter::Dead()
{
	UE_LOG(LogTemp, Warning, TEXT("minions dead"));

	// 死亡フラグ
	bIsDead = true;

	// AI停止
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		if (AI->BrainComponent)
		{
			AI->BrainComponent->StopLogic(TEXT("Dead"));
		}
	}

	// コリジョン停止
	SetActorEnableCollision(false);

	// 非表示
	SetActorHiddenInGame(true);

	if (AttackComponent)
	{
		AttackComponent->SetShowDebug(false);
	}
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


		AttackTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Normal")));


		AttackTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Middle")));


		AttackTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Strong")));



		AbilitySystemComponent->CancelAbilities(&AttackTags);

	}


	UE_LOG(LogTemp, Warning,
		TEXT("MINION ATTACK CANCEL"));

}

void AMinionsCharacter::SetCanSpawnOrb(bool bEnable)
{
	bCanSpawnOrb = bEnable;

	UE_LOG(LogTemp, Warning,
		TEXT("SetCanSpawnOrb called : %s -> %d"),
		*GetName(),
		bCanSpawnOrb
	);
}

void AMinionsCharacter::UpdateHPWidgetVisibility()
{
	if (!HPWidgetComponent)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn)
	{
		return;
	}

	const float DistanceSquared =FVector::DistSquared(GetActorLocation(),PlayerPawn->GetActorLocation());

	const float VisibleDistanceSquared =FMath::Square(HPWidgetVisibleDistance);

	const bool bShouldShow =DistanceSquared <= VisibleDistanceSquared;

	HPWidgetComponent->SetVisibility(bShouldShow);
}

void AMinionsCharacter::SetAttackInProgress(bool bInProgress)
{
	bIsAttackInProgress = bInProgress;
}