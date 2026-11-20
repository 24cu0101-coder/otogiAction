#include "LongRangeMinion.h"
#include "ArrowActor.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "OtogiAction/UI/EnemyHPWidget.h"

ALongRangeMinion::ALongRangeMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(RootComponent);
	MuzzlePoint->SetRelativeLocation(FVector(60.0f, 0.0f, 40.0f));

	HealthBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComp->SetupAttachment(RootComponent);
	HealthBarWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	HealthBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComp->SetDrawSize(FVector2D(120.0f, 15.0f));

	// 武器メッシュの作成とメッシュへのアタッチ
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALongRangeMinion::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	if (HealthBarWidgetComp)
	{
		HealthBarWidget = Cast<UEnemyHPWidget>(HealthBarWidgetComp->GetUserWidgetObject());
	}

	if (WeaponMesh && GetMesh() && WeaponSocketName != NAME_None)
	{
		WeaponMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,WeaponSocketName);
	}

	UpdateHealthBar(1.0f);

	GetWorldTimerManager().SetTimer(FireTimerHandle,this,&ALongRangeMinion::StartAttack,FireInterval,true);
}

void ALongRangeMinion::UpdateHealthBar(float HealthPercent)
{
	if (!HealthBarWidget && HealthBarWidgetComp)
	{
		HealthBarWidget = Cast<UEnemyHPWidget>(HealthBarWidgetComp->GetUserWidgetObject());
	}

	if (!HealthBarWidget)
	{
		return;
	}

	HealthBarWidget->SetHP(CurrentHealth, MaxHealth);
}

void ALongRangeMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	if (Distance <= AttackRange)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
		FRotator TargetRot = FRotator(0.0f, LookAtRot.Yaw, 0.0f);
		FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, RotationInterpSpeed);
		SetActorRotation(NewRot);
	}
}

void ALongRangeMinion::StartAttack()
{
	// いずれかの攻撃フェーズ実行中なら重複開始しない
	if (bIsDead || bIsPreparing || bIsAiming || bIsAttacking || !ArrowClass)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	if (Distance > AttackRange) return;

	// 1段階目：Prepare（弓を引く）
	if (PrepareMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		bIsPreparing = true;

		float Duration = GetMesh()->GetAnimInstance()->Montage_Play(PrepareMontage);
		if (Duration > 0.0f)
		{
			// ブレンドアウト直前に次の Aim へ繋いでポーズの途切れを防ぐ
			float NextDelay = FMath::Max(0.05f, Duration - 0.1f);
			GetWorldTimerManager().SetTimer(	PrepareTimerHandle,	this,&ALongRangeMinion::FinishPrepare,NextDelay,	false);
		}
		else
		{
			FinishPrepare();
		}
	}
	else
	{
		FinishPrepare();
	}
}

void ALongRangeMinion::FinishPrepare()
{
	if (bIsDead) return;

	bIsPreparing = false;

	// 2段階目：Aim（狙い続ける）
	if (AimMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		bIsAiming = true;

		GetMesh()->GetAnimInstance()->Montage_Play(AimMontage);

		GetWorldTimerManager().SetTimer(	AimTimerHandle,	this,	&ALongRangeMinion::FinishAim,	AimDuration,	false);
	}
	else
	{
		FinishAim();
	}
}

void ALongRangeMinion::FinishAim()
{
	if (bIsDead) return;

	bIsAiming = false;
	StartAttackMontage();
}

void ALongRangeMinion::StartAttackMontage()
{
	if (bIsDead) return;

	// 3段階目：Attack（矢を放つモーション再生 → 中のAnimNotifyでSpawnArrowが呼ばれる）
	if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		bIsAttacking = true;

		float Duration = GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
		if (Duration > 0.0f)
		{
			GetWorldTimerManager().SetTimer(AttackTimerHandle,this,&ALongRangeMinion::FinishAttack,Duration,		false);
		}
		else
		{
			SpawnArrow();
			FinishAttack();
		}
	}
	else
	{
		SpawnArrow();
		FinishAttack();
	}
}

void ALongRangeMinion::FinishAttack()
{
	bIsAttacking = false;
}

void ALongRangeMinion::SpawnArrow()
{
	if (bIsDead || !ArrowClass) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector SpawnLocation = MuzzlePoint->GetComponentLocation();
	if (GetMesh() && MuzzleSocketName != NAME_None && GetMesh()->DoesSocketExist(MuzzleSocketName))
	{
		SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	}

	FVector TargetLocation = PlayerPawn->GetActorLocation();
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AArrowActor>(ArrowClass, SpawnLocation, SpawnRotation, SpawnParams);
}

float ALongRangeMinion::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead || DamageAmount <= 0.0f)
	{
		return 0.0f;
	}

	const float ActualDamage = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,	DamageCauser);

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);

	float HealthPercent = (MaxHealth > 0.0f) ? CurrentHealth / MaxHealth : 0.0f;
	UpdateHealthBar(HealthPercent);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	return ActualDamage;
}

void ALongRangeMinion::Die()
{
	if (bIsDead) return;

	bIsDead = true;

	// すべての攻撃・状態遷移タイマーを停止
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	GetWorldTimerManager().ClearTimer(PrepareTimerHandle);
	GetWorldTimerManager().ClearTimer(AimTimerHandle);
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);

	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (HealthBarWidgetComp)
	{
		HealthBarWidgetComp->SetVisibility(false);
	}

	OnDeath();
	SetLifeSpan(DestroyDelay);
}