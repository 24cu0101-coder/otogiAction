//担当：飯島
//ターゲット時のロックUI

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetLockWidget.generated.h"

//前方宣言
class UImage;
class UCameraSystemComponent;

UCLASS()
class OTOGIACTION_API UTargetLockWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// ターゲットを設定
	void SetTarget(AActor* NewTarget);

	// ターゲット解除
	void ClearTarget();

	// ターゲット位置を更新
	void UpdateTargetPosition();

	// ロックオン演出開始
	void PlayLockOnEffect();

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(
		const FGeometry& MyGeometry,
		float InDeltaTime) override;

protected:

	// アウトライン
	UPROPERTY(meta = (BindWidget))
	UImage* OuterDiamond;

	// センターポイント
	UPROPERTY(meta = (BindWidget))
	UImage* CenterPoint;

	// ターゲット
	UPROPERTY()
	AActor* TargetActor;

	//カメラシステムコンポーネント
	UPROPERTY()
	UCameraSystemComponent* CameraSystemComp;


	// 演出時間
	UPROPERTY(EditAnywhere, Category = "Target Pointer")
	float EffectDuration = 0.25f;

private:

	// 演出経過時間
	float EffectTime;

	// 演出中か
	bool bPlayingEffect;
	
};
