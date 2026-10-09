//担当：飯島


#include "CameraDirectorComponent.h"

#include "../../DataAsset/Camera/ActionCameraSettingDataAsset.h"
#include "../../DataAsset/Camera/Modifier/CameraModifierDataAsset.h"

#include "../../PlayerCharacter.h"
#include "../PlayerTargetComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

//========================
//コンストラクタ
//========================
UCameraDirectorComponent::UCameraDirectorComponent()
{
	//Tickを有効化に設定
	PrimaryComponentTick.bCanEverTick = true;
}


//===========================
//ゲーム始まった時に処理される
//===========================
void UCameraDirectorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//プレイヤーを取得
	m_PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	if (!m_PlayerCharacter) return;

	//ターゲットコンポーネント
	m_PlayerTargetComponent = m_PlayerCharacter->FindComponentByClass<UPlayerTargetComponent>();

	//SpringArmコンポーネント
	m_SpringArmComponent = m_PlayerCharacter->FindComponentByClass<USpringArmComponent>();

	//カメラコンポーネント
	m_CameraComponent = m_PlayerCharacter->FindComponentByClass<UCameraComponent>();

	//初期のカメラ設定
	m_CurrentCameraSetting = m_GameplayCameraSetting;

}

//=======================
//毎フレーム処理される
//=======================
void UCameraDirectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);

	//アクションカメラが無効なら終了
	if (!bActionCameraEnabled)return;

	UpdateActionCamera(DeltaTime);

	ApplyModifier(DeltaTime);
}

//=======================
//アクションカメラを使っているか
//=======================
void UCameraDirectorComponent::SetActionCameraEnabled(bool bEnabled)
{
	bActionCameraEnabled = bEnabled;
}

//=======================
//ボス戦カメラを使用しているか
//=======================
void UCameraDirectorComponent::SetBossCameraEnabled(bool bEnabled)
{
	bBossCameraEnabled = bEnabled;
}

//=======================
//アクションカメラを設定
//=======================
void UCameraDirectorComponent::SetActionCameraSetting(UActionCameraSettingDataAsset* NewSetting)
{
	if (!NewSetting)
	{
		return;
	}

	m_CurrentCameraSetting = NewSetting;
}

//=======================
//カメラ設定
//=======================
void UCameraDirectorComponent::SetModifier(UCameraModifierDataAsset* Modifier, float Weight)
{
	if (!Modifier)
	{
		return;
	}


	for (FActiveModifier& Active : ActiveModifiers)
	{
		if (Active.Data == Modifier)
		{
			Active.Weight = Weight;
			return;
		}
	}


	FActiveModifier NewModifier;

	NewModifier.Data = Modifier;
	NewModifier.Weight = Weight;

	ActiveModifiers.Add(NewModifier);
}

//=======================
//カメラ設定を解除
//=======================
void UCameraDirectorComponent::ClearModifier(UCameraModifierDataAsset* Modifier)
{
	if (!Modifier)
	{
		return;
	}


	ActiveModifiers.RemoveAll(
		[Modifier](const FActiveModifier& Active)
		{
			return Active.Data == Modifier;
		}
	);
}

//=======================
//全てのカメラ設定を解除
//=======================
void UCameraDirectorComponent::ClearAllModifiers()
{
	ActiveModifiers.Empty();
}

//=======================
//アクションカメラの更新処理
//=======================
void UCameraDirectorComponent::UpdateActionCamera(float DeltaTime)
{
	//NULLチェック
	if (!m_PlayerCharacter || !m_SpringArmComponent || !m_CameraComponent) return;

	//カメラセッティング
	//ターゲット設定
	UActionCameraSettingDataAsset* TargetSetting = m_GameplayCameraSetting;

	//ボス戦闘用カメラ設定かターゲット用カメラ設定を設定する
	if (bBossCameraEnabled && m_BossCameraSetting)
	{
		TargetSetting = m_BossCameraSetting;
	}
	else if (m_PlayerTargetComponent && m_PlayerTargetComponent->IsTargeting() && m_LockOnCameraSetting)
	{
		TargetSetting = m_LockOnCameraSetting;
	}

	//カメラ設定がなければ終了
	if (!TargetSetting)return;

	//現在のカメラ設定を更新
	m_CurrentCameraSetting = TargetSetting;
	
	//通常カメラ
	if (!m_PlayerTargetComponent || !m_PlayerTargetComponent->IsTargeting())
	{
		m_SpringArmComponent->TargetArmLength = FMath::FInterpTo(m_SpringArmComponent->TargetArmLength, TargetSetting->TargetArmLength, DeltaTime, TargetSetting->DistanceInterpSpeed);

		m_CameraComponent->FieldOfView = FMath::FInterpTo(m_CameraComponent->FieldOfView, TargetSetting->FOV, DeltaTime, TargetSetting->FOVInterpSpeed);

		return;
	}

	//ターゲットロック
	AActor* TargetActor = m_PlayerTargetComponent->GetCurrentTargetActor();

	if (!TargetActor)return;

	//カメラフォーカス点
	const FVector FocusPoint = CalculateCombatFocusPoint();

	//カメラの距離
	const float TargetDistance =CalculateCombatCameraDistance();

	m_SpringArmComponent->TargetArmLength = FMath::FInterpTo(m_SpringArmComponent->TargetArmLength, TargetDistance, DeltaTime, TargetSetting->DistanceInterpSpeed);

	//カメラ設定オフセット
	m_SpringArmComponent->SocketOffset = FMath::VInterpTo(m_SpringArmComponent->SocketOffset, TargetSetting->SocketOffset, DeltaTime, TargetSetting->DistanceInterpSpeed);

	//ターゲット中カメラ回転
	UpdateCombatRotation(FocusPoint, DeltaTime);

	//視野角
	m_CameraComponent->FieldOfView = FMath::FInterpTo(m_CameraComponent->FieldOfView, TargetSetting->FOV, DeltaTime, TargetSetting->FOVInterpSpeed);
}

//=======================
//注視点
//=======================
FVector UCameraDirectorComponent::CalculateCombatFocusPoint() const
{
	if (!m_PlayerCharacter || !m_PlayerTargetComponent || !m_CurrentCameraSetting)
	{
		return FVector::ZeroVector;
	}

	AActor* TargetActor = m_PlayerTargetComponent->GetCurrentTargetActor();

	if (!TargetActor)
	{
		return m_PlayerCharacter->GetActorLocation();
	}

	//プレイヤー側注視点
	FVector PlayerPoint = m_PlayerCharacter->GetActorLocation();

	PlayerPoint.Z += m_CurrentCameraSetting->PlayerFocusHeight;

	//エネミー側注視点
	FVector EnemyPoint = TargetActor->GetActorLocation();
	EnemyPoint.Z = m_CurrentCameraSetting->EnemyFocusHeight;

	//中心点の計算
	const float EnemyWeight = FMath::Clamp(m_CurrentCameraSetting->TargetFocusWeight, 0.0f, 1.0f);

	const float PlayerWeight = 1.0f - EnemyWeight;

	return PlayerPoint * PlayerWeight + EnemyPoint * EnemyWeight;
}

//==========================
//敵との距離によってカメラの距離変える
//==========================
float UCameraDirectorComponent::CalculateCombatCameraDistance() const
{
	if (!m_PlayerCharacter || !m_PlayerTargetComponent || !m_CurrentCameraSetting)
	{
		return 400.f;
	}

	AActor* TargetActor = m_PlayerTargetComponent->GetCurrentTargetActor();

	if (!TargetActor)
	{
		return m_CurrentCameraSetting->TargetArmLength;
	}
	//プレイヤーと敵の距離
	const float Distance = FVector::Dist(m_PlayerCharacter->GetActorLocation(), TargetActor->GetActorLocation());

	//スプリングアームの距離
	float TargetArmLength = m_CurrentCameraSetting->TargetArmLength + Distance * m_CurrentCameraSetting->DistanceScale;

	return FMath::Clamp(TargetArmLength, m_CurrentCameraSetting->MinArmLength, m_CurrentCameraSetting->MaxArmLength);
}

//===============================================
//ターゲット中フォーカスにむかってカメラを回転させる
//===============================================
void UCameraDirectorComponent::UpdateCombatRotation(const FVector& FocusPoint, float DeltaTime)
{
	if (!m_PlayerCharacter || !m_PlayerTargetComponent || !m_CurrentCameraSetting) return;

	const FVector CameraLocation = m_CameraComponent->GetComponentLocation();

	FVector LookDirection = FocusPoint - CameraLocation;

	if (LookDirection.IsNearlyZero())return;

	FRotator TargetRotation = LookDirection.Rotation();

	//カメラの上下移動制限
	TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch, -60.f, 60.f);


	//カメラ回転補間
	const FRotator CurrentRotation = m_SpringArmComponent->GetComponentRotation();
	
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, m_CurrentCameraSetting->RotationInterpSpeed);

	m_SpringArmComponent->SetWorldRotation(NewRotation);
}

void UCameraDirectorComponent::ApplyModifier(float DeltaTime)
{
	if (!m_SpringArmComponent || !m_CameraComponent) return;

	float ArmLengthOffset = 0.f;

	FVector SocketOffset = FVector::ZeroVector;

	float FOVOffset = 0.f;

	float InterpSpeed = 8.f;

	for (const FActiveModifier& Active : ActiveModifiers)
	{
		if (!Active.Data || Active.Weight <= 0.0f)
		{
			continue;
		}


		const float Weight = FMath::Clamp(Active.Weight, 0.0f, 1.0f);

		ArmLengthOffset += Active.Data->TargetArmLengthOffset * Weight;

		SocketOffset += Active.Data->SocketOffset * Weight;


		FOVOffset += Active.Data->FOVOffset * Weight;


		InterpSpeed = FMath::Max( InterpSpeed, Active.Data->InterpSpeed);
	}

	//距離
	const float TargetArmLength = m_SpringArmComponent->TargetArmLength + ArmLengthOffset;

	m_SpringArmComponent->TargetArmLength = FMath::FInterpTo(m_SpringArmComponent->TargetArmLength, TargetArmLength, DeltaTime, InterpSpeed);

	//カメラオフセット
	m_SpringArmComponent->SocketOffset = FMath::VInterpTo(m_SpringArmComponent->SocketOffset, SocketOffset, DeltaTime, InterpSpeed);

	//視野角

	const float BaseFOV = m_CurrentCameraSetting ? m_CurrentCameraSetting->FOV : 90.0f;

	const float TargetFOV = BaseFOV + FOVOffset;

	m_CameraComponent->FieldOfView = FMath::FInterpTo( m_CameraComponent->FieldOfView, TargetFOV, DeltaTime, InterpSpeed);
}


