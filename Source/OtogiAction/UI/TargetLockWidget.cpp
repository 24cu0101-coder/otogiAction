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

	EffectTime = 0.0f;
	bPlayingEffect = false;
	CameraSystemComp = nullptr;

	// 最初は非表示
	if (OuterDiamond)
	{
		OuterDiamond->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CenterPoint)
	{
		CenterPoint->SetVisibility(ESlateVisibility::Hidden);
	}

	// カメラシステム取得
	APlayerController* PlayerController = GetOwningPlayer();

	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();

		if (PlayerPawn)
		{
			CameraSystemComp = PlayerPawn->FindComponentByClass<UCameraSystemComponent>();
		}
	}
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

		float Alpha =
			FMath::Clamp(
				EffectTime / EffectDuration,
				0.0f,
				1.0f
			);

		// 最初は大きく
		// 徐々に小さくする
		float Scale =
			FMath::Lerp(
				1.8f,
				0.8f,
				Alpha
			);

		if (OuterDiamond)
		{
			OuterDiamond->SetRenderScale(
				FVector2D(Scale, Scale)
			);

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
				OuterDiamond->SetVisibility(
					ESlateVisibility::Hidden
				);
			}
		}
	}
}


void UTargetLockWidget::SetTarget(AActor* NewTarget)
{
	TargetActor = NewTarget;

	if (!TargetActor)
	{
		ClearTarget();
		return;
	}

	// センターポイントは最初は隠す
	if (CenterPoint)
	{
		CenterPoint->SetVisibility(
			ESlateVisibility::Hidden
		);
	}

	// アウトラインを表示
	if (OuterDiamond)
	{
		OuterDiamond->SetVisibility(
			ESlateVisibility::Visible
		);
	}

	PlayLockOnEffect();
}

void UTargetLockWidget::ClearTarget()
{
	TargetActor = nullptr;

	bPlayingEffect = false;

	if (OuterDiamond)
	{
		OuterDiamond->SetVisibility(
			ESlateVisibility::Hidden
		);
	}

	if (CenterPoint)
	{
		CenterPoint->SetVisibility(
			ESlateVisibility::Hidden
		);
	}
}

void UTargetLockWidget::UpdateTargetPosition()
{
	if (!TargetActor)return;

	APlayerController* PlayerController = GetOwningPlayer();

	if(!PlayerController)return;

	// 敵の頭あたりを狙う
	FVector WorldLocation = CameraSystemComp->GetEnemyTargetPoint();

	FVector2D ScreenPosition;

	bool bProjected = PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition, true);

	if (!bProjected)return;

	// Widget自身をターゲット位置へ移動
	SetPositionInViewport(ScreenPosition, false);
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



