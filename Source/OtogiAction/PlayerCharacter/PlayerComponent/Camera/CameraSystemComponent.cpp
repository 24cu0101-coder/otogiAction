// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSystemComponent.h"
#include "MoveCameraComponent.h"
#include "../PlayerTargetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

//コンストラクタ
UCameraSystemComponent::UCameraSystemComponent()
{
	//Tickは無効化
	PrimaryComponentTick.bCanEverTick = true;

	MoveCameraComp = nullptr;
	TargetComp = nullptr;

	SpringArmComp = nullptr;
	CameraComp = nullptr;

	// 初期状態は通常カメラ
	CurrentCameraMode = ECameraMode::Normal;

	//==================================================
	// ノーマルカメラ設定
	//==================================================

	NormalCameraSetting.ArmLength = 400.0f;
	NormalCameraSetting.FOV = 90.0f;
	NormalCameraSetting.SocketOffset = FVector::ZeroVector;
	NormalCameraSetting.InterpSpeed = 8.0f;


	//==================================================
	// ターゲットカメラ設定
	//==================================================

	TargetLockCameraSetting.ArmLength = 400.0f;
	TargetLockCameraSetting.FOV = 90.0f;
	TargetLockCameraSetting.SocketOffset = FVector::ZeroVector;
	TargetLockCameraSetting.InterpSpeed = 8.0f;


	//==================================================
	// ボス戦闘カメラ設定
	//==================================================

	BossBattleCameraSetting.ArmLength = 500.0f;
	BossBattleCameraSetting.FOV = 85.0f;
	BossBattleCameraSetting.SocketOffset = FVector::ZeroVector;
	BossBattleCameraSetting.InterpSpeed = 5.0f;
}


// 生成されたとき処理される
void UCameraSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (!OwnerCharacter)return;

	SpringArmComp = OwnerCharacter->FindComponentByClass<USpringArmComponent>();

	CameraComp = OwnerCharacter->FindComponentByClass<UCameraComponent>();
	
}

void UCameraSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);

	//カメラ設定適応
	ApplyCameraSetting(DeltaTime);

	//カメラモード
	switch (CurrentCameraMode)
	{
	case ECameraMode::Normal:

		if (bReturningToNormalCamera)
		{
			UpdateReturnToNormalCamera(DeltaTime);
		}
		
		break;


	case ECameraMode::TargetLock:

		UpdateTargetLockCamera(DeltaTime);

		break;


	case ECameraMode::BossBattle:

		break;
	}
}


void UCameraSystemComponent::SetMoveCameraComponent(UMoveCameraComponent* CameraComponent)
{
	MoveCameraComp = CameraComponent;
}

void UCameraSystemComponent::ApplyCameraSetting(float DeltaTime)
{
	if (!SpringArmComp || !CameraComp) return;

	// TargetLock中はSpringArmを使用しない
	if (CurrentCameraMode == ECameraMode::TargetLock)
	{
		CameraComp->FieldOfView =FMath::FInterpTo(CameraComp->FieldOfView,TargetLockCameraSetting.FOV,DeltaTime,TargetLockCameraSetting.InterpSpeed);
		return;
	}

	FCameraSetting Setting = GetCurrentCameraSetting();

	SpringArmComp->TargetArmLength =FMath::FInterpTo(SpringArmComp->TargetArmLength,Setting.ArmLength,DeltaTime,Setting.InterpSpeed);

	CameraComp->FieldOfView =FMath::FInterpTo(CameraComp->FieldOfView,Setting.FOV,DeltaTime,Setting.InterpSpeed);

	SpringArmComp->SocketOffset =FMath::VInterpTo(SpringArmComp->SocketOffset,Setting.SocketOffset,DeltaTime,Setting.InterpSpeed);
}

void UCameraSystemComponent::CameraMove(const FVector2D& InputValue)
{
	if (!MoveCameraComp) return;

	// 通常カメラへ復帰中は操作しない
	if (bReturningToNormalCamera) return;

	MoveCameraComp->CameraMove(InputValue);
}

void UCameraSystemComponent::TargetChange(const FVector2D& InputValue)
{
	if (!TargetComp)return;

	// ターゲット中でなければ変更しない
	if (!TargetComp->IsTargeting())return;

	// 入力がなければ返す
	if (InputValue.IsNearlyZero())return;

	TargetComp->ChangeTarget(InputValue);

	UE_LOG(LogTemp,Warning,TEXT("TargetChangeInput : X=%f Y=%f"),InputValue.X,InputValue.Y);
}

void UCameraSystemComponent::ToggleTargetLock()
{
	if (!TargetComp)return;

	TargetComp->ToggleTargetLock();

	if (TargetComp->IsTargeting())
	{
		// TargetLock開始
		if (CameraComp)
		{
			CameraComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}

		SetCameraMode(ECameraMode::TargetLock);

		UE_LOG(LogTemp, Warning, TEXT("Camera Target Start"));
	}
	else
	{
		// TargetLock終了
		SetCameraMode(ECameraMode::Normal);

		//通常カメラの復帰先を取得
		if (CameraComp)
		{
			const FRotator CurrentCameraRotation =CameraComp->GetComponentRotation();

			NormalCameraReturnTargetRotation =FRotator(-20.0f,CurrentCameraRotation.Yaw,0.0f);
		}
		
		// カメラを通常位置まで戻す
		bReturningToNormalCamera = true;

		UE_LOG(LogTemp,Warning,TEXT("Camera Return Start"));
	}
}

//ノーマルカメラにリセット
void UCameraSystemComponent::ResetNormalCamera()
{
	if (!SpringArmComp || !CameraComp)return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (!OwnerCharacter)return;

	// CameraをSpringArmへ戻す
	CameraComp->AttachToComponent(SpringArmComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// Camera自身のTransformを完全リセット
	CameraComp->SetRelativeLocation(FVector::ZeroVector);
	CameraComp->SetRelativeRotation(FRotator::ZeroRotator);

	// 通常カメラの回転
	const float PlayerYaw = OwnerCharacter->GetActorRotation().Yaw;

	// Playerの向きを基準にする
	// Pitchは少し上から見る
	FRotator NormalCameraRotation(-20.0f,PlayerYaw,0.0f);

	SpringArmComp->SetWorldRotation(NormalCameraRotation);

	// 通常カメラ設定
	SpringArmComp->TargetArmLength = NormalCameraSetting.ArmLength;

	SpringArmComp->SocketOffset = NormalCameraSetting.SocketOffset;

	CameraComp->FieldOfView = NormalCameraSetting.FOV;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Normal Camera Reset : PlayerYaw=%f / SpringArm=%s / Camera=%s"),
		PlayerYaw,
		*SpringArmComp->GetComponentRotation().ToString(),
		*CameraComp->GetComponentLocation().ToString()
	);
}

void UCameraSystemComponent::UpdateTargetLockCamera(float DeltaTime)
{
	if (!TargetComp || !CameraComp)return;

	AActor* TargetActor = TargetComp->GetCurrentTargetActor();

	if (!IsValid(TargetActor))
	{
		// ターゲットが死亡・破棄された
		TargetComp->ToggleTargetLock();

		SetCameraMode(ECameraMode::Normal);

		const FRotator CurrentCameraRotation =CameraComp->GetComponentRotation();

		NormalCameraReturnTargetRotation =FRotator(-20.0f,CurrentCameraRotation.Yaw,0.0f);

		bReturningToNormalCamera = true;

		UE_LOG(LogTemp,Warning,TEXT("===== TARGET INVALID : TARGET LOCK END ====="));

		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (!OwnerCharacter) return;

	// Player位置
	FVector PlayerLocation = OwnerCharacter->GetActorLocation();

	PlayerLocation.Z += PlayerTargetPointHeight;


	// Enemy位置
	FVector TargetLocation = GetEnemyTargetPoint();

	// 2人の中心
	FVector CenterLocation = (PlayerLocation + TargetLocation) * 0.5f;


	// PlayerとEnemyの距離

	float Distance =FVector::Distance(PlayerLocation,TargetLocation);

	// 最大距離を超えたらTargetLock終了
	if (Distance >= MaxTargetCameraDistance)
	{
		TargetComp->ToggleTargetLock();

		SetCameraMode(ECameraMode::Normal);

		//復帰後の
		if (CameraComp)
		{
			const FRotator CurrentCameraRotation =CameraComp->GetComponentRotation();

			NormalCameraReturnTargetRotation =FRotator(-20.0f,CurrentCameraRotation.Yaw,0.0f);
		}

		bReturningToNormalCamera = true;

		UE_LOG(LogTemp,Warning,TEXT("Target Lock End : Distance = %f / MaxDistance = %f"),Distance,MaxTargetCameraDistance);

		return;
	}


	// カメラ距離
	float CameraDistance =FMath::Clamp(Distance * 1.2f,MinTargetCameraDistance,MaxTargetCameraDistance);


	// Player → Enemy

	FVector Direction = (TargetLocation - PlayerLocation).GetSafeNormal();

	if (Direction.IsNearlyZero())return;


	// カメラを2人の後ろに配置
	FVector CameraDirection = -Direction;

	CameraDirection.Z = 0.0f;
	CameraDirection.Normalize();

	// カメラ位置
	FVector DesiredCameraLocation = CenterLocation + CameraDirection * CameraDistance;

	// カメラ高さ
	DesiredCameraLocation.Z += 100.0f;


	// 中心を見る
	FVector LookDirection = CenterLocation - DesiredCameraLocation;

	FRotator DesiredRotation = LookDirection.Rotation();


	// カメラ移動
	const float InterpSpeed = TargetLockCameraSetting.InterpSpeed;

	FVector NewLocation =FMath::VInterpTo(CameraComp->GetComponentLocation(),DesiredCameraLocation,DeltaTime,InterpSpeed);


	FRotator NewRotation =FMath::RInterpTo(CameraComp->GetComponentRotation(),DesiredRotation,DeltaTime,InterpSpeed);


	CameraComp->SetWorldLocation(NewLocation);
	CameraComp->SetWorldRotation(NewRotation);
}

void UCameraSystemComponent::SetCameraMode(ECameraMode NewMode)
{
	if (CurrentCameraMode == NewMode)
	{
		return;
	}

	CurrentCameraMode = NewMode;

	UE_LOG(LogTemp,Log,TEXT("Camera Mode Changed : %d"),static_cast<int32>(CurrentCameraMode));
}

//ターゲットをセット
void UCameraSystemComponent::SetTargetComponent(UPlayerTargetComponent* TargetComponent)
{
	TargetComp = TargetComponent;
}

//カメラを通常状態に戻す処理
void UCameraSystemComponent::UpdateReturnToNormalCamera(float DeltaTime)
{
	if (!SpringArmComp || !CameraComp)return;

	// SpringArm回転
	const FRotator CurrentSpringRotation =SpringArmComp->GetComponentRotation();

	const FRotator DesiredSpringRotation =NormalCameraReturnTargetRotation;

	const FRotator NewSpringRotation =FMath::RInterpTo(CurrentSpringRotation,DesiredSpringRotation,DeltaTime,NormalCameraReturnInterpSpeed);

	SpringArmComp->SetWorldRotation(NewSpringRotation);

	// SpringArm設定
	SpringArmComp->TargetArmLength =FMath::FInterpTo(SpringArmComp->TargetArmLength,NormalCameraSetting.ArmLength,DeltaTime,NormalCameraReturnInterpSpeed);

	SpringArmComp->SocketOffset =FMath::VInterpTo(SpringArmComp->SocketOffset,NormalCameraSetting.SocketOffset,DeltaTime,NormalCameraReturnInterpSpeed);

	// CameraをSpringArmへ追従
	const FVector DesiredCameraLocation =SpringArmComp->GetSocketLocation(TEXT("SpringEndpoint"));

	const FRotator DesiredCameraRotation =SpringArmComp->GetSocketRotation(TEXT("SpringEndpoint"));

	const FVector NewCameraLocation =FMath::VInterpTo(CameraComp->GetComponentLocation(),DesiredCameraLocation,DeltaTime,NormalCameraReturnInterpSpeed);

	const FRotator NewCameraRotation =FMath::RInterpTo(CameraComp->GetComponentRotation(),DesiredCameraRotation,DeltaTime,NormalCameraReturnInterpSpeed);

	CameraComp->SetWorldLocation(NewCameraLocation);
	CameraComp->SetWorldRotation(NewCameraRotation);

	// 復帰完了判定
	const float RotationDistance =FMath::Abs(FMath::FindDeltaAngleDegrees(SpringArmComp->GetComponentRotation().Yaw,NormalCameraReturnTargetRotation.Yaw));

	const float PitchDistance =FMath::Abs(SpringArmComp->GetComponentRotation().Pitch -NormalCameraReturnTargetRotation.Pitch);

	const float ArmLengthDistance =FMath::Abs(SpringArmComp->TargetArmLength -NormalCameraSetting.ArmLength);

	if (RotationDistance < 1.0f &&PitchDistance < 1.0f &&ArmLengthDistance < 1.0f)
	{
		// 復帰修了
		CameraComp->AttachToComponent(SpringArmComp,FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		CameraComp->SetRelativeLocation(FVector::ZeroVector);

		CameraComp->SetRelativeRotation(FRotator::ZeroRotator);

		CameraComp->FieldOfView =NormalCameraSetting.FOV;

		bReturningToNormalCamera = false;

		UE_LOG(LogTemp,Warning,TEXT("===== CAMERA RETURN COMPLETE ====="));
	}
}

//ターゲットを取得
AActor* UCameraSystemComponent::GetCurrentTarget() const
{
	if (!TargetComp)
	{
		return nullptr;
	}

	return TargetComp->GetCurrentTargetActor();
}

FVector UCameraSystemComponent::GetEnemyTargetPoint() const
{
	if (!TargetComp)
	{
		return FVector::ZeroVector;
	}

	AActor* TargetActor = TargetComp->GetCurrentTargetActor();

	if (!TargetActor)
	{
		return FVector::ZeroVector;
	}

	FVector TargetLocation = TargetActor->GetActorLocation();

	TargetLocation.Z += EnemyTargetPointHeight;

	return TargetLocation;
}

//ターゲットしているか
bool UCameraSystemComponent::IsTargeting() const
{
	if (!TargetComp)
	{
		return false;
	}

	return TargetComp->IsTargeting();
}

FCameraSetting UCameraSystemComponent::GetCurrentCameraSetting() const
{
	switch (CurrentCameraMode)
	{
	case ECameraMode::Normal:
		return NormalCameraSetting;


	case ECameraMode::TargetLock:
		return TargetLockCameraSetting;


	case ECameraMode::BossBattle:
		return BossBattleCameraSetting;
	}

	return NormalCameraSetting;
}
