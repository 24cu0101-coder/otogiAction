////担当：飯島
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "CameraDirectorComponent.generated.h"
//
////前方宣言
//class UActionCameraSettingDataAsset;
//class UCameraModifierDataAsset;
//class ULevelSequence;
//class ULevelSequencePlayer;
//class APlayerCharacter;
//class UMoveCameraComponent;
//class USpringArmComponent;
//class UCameraComponent;
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class OTOGIACTION_API UCameraDirectorComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:
//	//コンストラクタ
//	UCameraDirectorComponent();
//
//protected:
//	// ゲーム開始時と生成時に呼ばれる
//	virtual void BeginPlay() override;
//
//public:
//	// 毎フレーム呼ばれる
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//
//	//==================================================
//	// Action Camera
//	//==================================================
//
//	UFUNCTION(BlueprintCallable, Category = "Camera|Action")
//	void SetActionCameraEnabled(bool bEnabled);
//
//
//	UFUNCTION(BlueprintCallable, Category = "Camera|Action")
//	void SetBossCameraEnabled(bool bEnabled);
//
//
//	UFUNCTION(BlueprintCallable, Category = "Camera|Action")
//	void SetActionCameraSetting(UActionCameraSettingDataAsset* NewSetting);
//
//
//	//==================================================
//	// Modifier
//	//==================================================
//
//	UFUNCTION(BlueprintCallable, Category = "Camera|Modifier")
//	void SetModifier(UCameraModifierDataAsset* Modifier,float Weight);
//
//
//	UFUNCTION(BlueprintCallable, Category = "Camera|Modifier")
//	void ClearModifier(UCameraModifierDataAsset* Modifier);
//
//
//	UFUNCTION(BlueprintCallable, Category = "Camera|Modifier")
//	void ClearAllModifiers();
//
//
//protected:
//
//	//==================================================
//	// Action Camera
//	//==================================================
//
//	void UpdateActionCamera(float DeltaTime);
//
//	//注視点付与
//	FVector CalculateCombatFocusPoint() const;
//
//	//カメラ距離の計算
//	float CalculateCombatCameraDistance() const;
//
//	//フォーカス点へカメラを向ける
//	void UpdateCombatRotation(const FVector& FocusPoint,float DeltaTime);
//
//	//==================================================
//	// Modifier
//	//==================================================
//
//	void ApplyModifier(float DeltaTime);
//
//private:
//	//==================================================
//	// ポインター
//	//==================================================
//
//	UPROPERTY()
//	TObjectPtr<APlayerCharacter> m_PlayerCharacter;
//
//	UPROPERTY()
//	TObjectPtr<UPlayerTargetComponent> m_PlayerTargetComponent;
//
//	UPROPERTY()
//	TObjectPtr<USpringArmComponent> m_SpringArmComponent;
//
//	UPROPERTY()
//	TObjectPtr<UCameraComponent> m_CameraComponent;
//
//
//	//==================================================
//	// カメラデータアセット
//	//==================================================
//
//	UPROPERTY(EditAnywhere, Category = "Camera|Action")
//	TObjectPtr<UActionCameraSettingDataAsset> m_GameplayCameraSetting;
//
//	UPROPERTY(EditAnywhere, Category = "Camera|Action")
//	TObjectPtr<UActionCameraSettingDataAsset> m_LockOnCameraSetting;
//
//	UPROPERTY(EditAnywhere, Category = "Camera|Action")
//	TObjectPtr<UActionCameraSettingDataAsset> m_BossCameraSetting;
//
//
//	UPROPERTY()
//	TObjectPtr<UActionCameraSettingDataAsset> m_CurrentCameraSetting;
//
//
//	//==================================================
//	// カメラステート
//	//==================================================
//
//	UPROPERTY(EditAnywhere, Category = "Camera|Action")
//	bool bActionCameraEnabled = true;
//
//	bool bBossCameraEnabled = false;
//
//
//	//==================================================
//	// Modifier
//	//==================================================
//
//	struct FActiveModifier
//	{
//		TObjectPtr<UCameraModifierDataAsset> Data = nullptr;
//
//		float Weight = 0.0f;
//	};
//
//	TArray<FActiveModifier> ActiveModifiers;
//};
