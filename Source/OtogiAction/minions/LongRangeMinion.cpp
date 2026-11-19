#include "LongRangeMinion.h"
#include "ArrowActor.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ALongRangeMinion::ALongRangeMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	// 移動を完全に無効化
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}

	// 矢の発射位置コンポーネントを作成
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(RootComponent);
	MuzzlePoint->SetRelativeLocation(FVector(60.0f, 0.0f, 40.0f));
}

void ALongRangeMinion::BeginPlay()
{
	Super::BeginPlay();

	// FireInterval 秒ごとに ShootArrow をループ実行
	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&ALongRangeMinion::ShootArrow,
		FireInterval,
		true
	);
}

void ALongRangeMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	// 射程内にプレイヤーがいる場合のみ向きを変える
	float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	if (Distance <= AttackRange)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());

		// キャラが上下に傾かないよう Yaw（水平回転）のみ抽出して滑らかに回転
		FRotator TargetRot = FRotator(0.0f, LookAtRot.Yaw, 0.0f);
		FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, RotationInterpSpeed);
		SetActorRotation(NewRot);
	}
}

void ALongRangeMinion::ShootArrow()
{
	if (!ArrowClass) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	// 射程外なら撃たない
	float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	if (Distance > AttackRange) return;

	// MuzzlePoint の位置からプレイヤーの座標への角度を計算
	FVector SpawnLocation = MuzzlePoint->GetComponentLocation();
	FVector TargetLocation = PlayerPawn->GetActorLocation();
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

	// スポーン設定（Ownerに自分を設定して自爆を防ぐ）
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AArrowActor>(ArrowClass, SpawnLocation, SpawnRotation, SpawnParams);
}