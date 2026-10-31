// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetLockWidget.h"
#include "Components/Image.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "../PlayerCharacter/PlayerComponent/Camera/CameraSystemComponent.h"

//コンストラクタ
void UTargetLockWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TargetActor = nullptr;
	CameraSystemComp = nullptr;

	EffectTime = 0.0f;
	bPlayingEffect = false;

	SetAlignmentInViewport(FVector2D(0.5f, 0.5f));

	if (OuterDiamond)
	{
		OuterDiamond->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CenterPoint)
	{
		CenterPoint->SetVisibility(ESlateVisibility::Hidden);
	}

	//==================================================
	// CameraSystemComponent取得
	//==================================================

	APlayerController* PlayerController = GetOwningPlayer();

	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();

		if (PlayerPawn)
		{
			CameraSystemComp =PlayerPawn->FindComponentByClass<UCameraSystemComponent>();
		}
	}

	UE_LOG(LogTemp,Warning,TEXT("TargetLockWidget : NativeConstruct / CameraSystemComp = %s"),CameraSystemComp? TEXT("VALID"): TEXT("NULL"));
}

//毎フレーム処理
void UTargetLockWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!TargetActor)
	{
		return;
	}

	UpdateTargetPosition();

	//========================================
	// ロックオン演出
	//========================================

	if (bPlayingEffect)
	{
		EffectTime += InDeltaTime;

		float Alpha =FMath::Clamp(EffectTime / EffectDuration,0.0f,1.0f);

		float Scale =FMath::Lerp(1.8f,0.8f,Alpha);

		if (OuterDiamond)
		{
			OuterDiamond->SetRenderScale(FVector2D(Scale, Scale));

			// 徐々に消す
			float Opacity =
				1.0f - Alpha;

			OuterDiamond->SetRenderOpacity(Opacity);
		}

		if (Alpha >= 1.0f)
		{
			bPlayingEffect = false;

			if (OuterDiamond)
			{
				OuterDiamond->SetVisibility(ESlateVisibility::Hidden);
			}

			if (CenterPoint)
			{
				CenterPoint->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}


void UTargetLockWidget::SetTarget(AActor* NewTarget)
{
	if (!IsValid(NewTarget))
	{
		ClearTarget();
		return;
	}

	TargetActor = NewTarget;

	// 毎回、表示状態を完全に初期化
	bPlayingEffect = false;
	EffectTime = 0.0f;

	if (OuterDiamond)
	{
		OuterDiamond->SetVisibility(ESlateVisibility::Visible);
		OuterDiamond->SetRenderScale(FVector2D(1.8f, 1.8f));
		OuterDiamond->SetRenderOpacity(1.0f);
	}

	if (CenterPoint)
	{
		CenterPoint->SetVisibility(ESlateVisibility::Hidden);
		CenterPoint->SetRenderOpacity(1.0f);
	}

	// ロックオン演出開始
	PlayLockOnEffect();

	UE_LOG(LogTemp,Warning,TEXT("TargetLockWidget : SetTarget / Target=%s"),*TargetActor->GetName());
}

void UTargetLockWidget::ClearTarget()
{
	TargetActor = nullptr;

	bPlayingEffect = false;
	EffectTime = 0.0f;

	if (OuterDiamond)
	{
		OuterDiamond->SetVisibility(ESlateVisibility::Hidden);
		OuterDiamond->SetRenderScale(FVector2D(1.0f, 1.0f));
		OuterDiamond->SetRenderOpacity(1.0f);
	}

	if (CenterPoint)
	{
		CenterPoint->SetVisibility(ESlateVisibility::Hidden);
		CenterPoint->SetRenderOpacity(1.0f);
	}

	UE_LOG(LogTemp,Warning,TEXT("TargetLockWidget : ClearTarget"));
}

void UTargetLockWidget::UpdateTargetPosition()
{
	if (!IsValid(TargetActor))return;

	if (!IsValid(CameraSystemComp))return;

	APlayerController* PlayerController = GetOwningPlayer();

	if (!PlayerController)return;

	// 敵の注視点を取得
	FVector WorldLocation =CameraSystemComp->GetEnemyTargetPoint();

	FVector2D ScreenPosition;

	bool bProjected =PlayerController->ProjectWorldLocationToScreen(WorldLocation,ScreenPosition,true);

	if (!bProjected)
	{
		return;
	}
	// Widgetの中心をターゲット位置に合わせる
	SetAlignmentInViewport(FVector2D(0.5f, 0.5f));

	SetPositionInViewport(ScreenPosition,true);
}

void UTargetLockWidget::PlayLockOnEffect()
{
	if (!OuterDiamond)
	{
		return;
	}

	EffectTime = 0.0f;
	bPlayingEffect = true;

	OuterDiamond->SetVisibility(
		ESlateVisibility::Visible
	);

	OuterDiamond->SetRenderScale(
		FVector2D(1.8f, 1.8f)
	);

	OuterDiamond->SetRenderOpacity(1.0f);
}



