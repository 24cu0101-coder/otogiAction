//担当：飯島
//カメラ操作

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraSystemComponent.generated.h"

//前方宣言
class UMoveCameraComponent;
class UPlayerTargetComponent;
class AActor;
class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	TargetLock	UMETA(DisplayName = "Target Lock"),
	BossBattle	UMETA(DisplayName = "Boss Battle")
};

//カメラ設定構造体
USTRUCT(BlueprintType)
struct FCameraSetting
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ArmLength = 400.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float FOV = 90.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FVector SocketOffset = FVector::ZeroVector;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float InterpSpeed = 8.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UCameraSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//コンストラクタ
	UCameraSystemComponent();

protected:
	//生成されたときに処理される
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// Camera
	//カメラ操作Componentを設定
	void SetMoveCameraComponent(UMoveCameraComponent* CameraComponent);

	//カメラ設定を適応
	void ApplyCameraSetting(float DeltaTime);

	//カメラ操作をおこなう関数
	void CameraMove(const FVector2D& InputValue);

	//ターゲット中のカメラ更新
	void UpdateTargetLockCamera(float DeltaTime);

	//ターゲット
	void ToggleTargetLock();

	//ノーマルカメラにリセット
	void ResetNormalCamera();

	//現在のカメラモードを変更
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetCameraMode(ECameraMode NewMode);

	//現在のカメラモードを取得
	UFUNCTION(BlueprintPure, Category = "Camera")
	ECameraMode GetCameraMode() const { return CurrentCameraMode; }

	// Target
	//ターゲットComponentを設定
	void SetTargetComponent(UPlayerTargetComponent* TargetComponent);

	// 通常カメラへ戻す処理
	void UpdateReturnToNormalCamera(float DeltaTime);

	//現在のターゲットを取得
	UFUNCTION(BlueprintPure, Category = "Camera|Target")
	AActor* GetCurrentTarget() const;

	//敵のターゲット注視点を取得
	UFUNCTION(BlueprintPure, Category = "Camera|Target")
	FVector GetEnemyTargetPoint() const;

	//ターゲット中か
	UFUNCTION(BlueprintPure, Category = "Camera|Target")
	bool IsTargeting() const;


private:
	// Camera
	//実際にカメラを操作するComponent
	UPROPERTY()
	TObjectPtr<UMoveCameraComponent> MoveCameraComp;

	// 現在のカメラモード
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	ECameraMode CurrentCameraMode;

	// 通常カメラへ戻っている途中か
	bool bReturningToNormalCamera = false;

	// 復帰先のSpringArm回転
	FRotator NormalCameraReturnTargetRotation;

	// 通常カメラへ戻る速度
	UPROPERTY(EditAnywhere, Category = "Camera|Transition")
	float NormalCameraReturnInterpSpeed = 8.0f;


	// Target
	//ターゲットを管理するComponent
	UPROPERTY()
	TObjectPtr<UPlayerTargetComponent> TargetComp;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComp;


	UPROPERTY(EditAnywhere, Category = "Camera|TargetLock")
	float PlayerTargetPointHeight = 80.0f;

	UPROPERTY(EditAnywhere, Category = "Camera|TargetLock")
	float EnemyTargetPointHeight = 80.0f;

	UPROPERTY(EditAnywhere, Category = "Camera|TargetLock")
	float TargetCameraInterpSpeed = 8.0f;

	UPROPERTY(EditAnywhere, Category = "Camera|TargetLock")
	float MinTargetCameraDistance = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Camera|TargetLock")
	float MaxTargetCameraDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Camera|TargetLock")
	float MaxTargetCameraPitch = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Camera|TargetLock")
	float MinTargetCameraPitch = -60.0f;
		

	// Camera Settings
	FCameraSetting GetCurrentCameraSetting() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Camera|Settings",meta = (AllowPrivateAccess = "true"))
	FCameraSetting NormalCameraSetting;


	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Camera|Settings",meta = (AllowPrivateAccess = "true"))
	FCameraSetting TargetLockCameraSetting;


	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Camera|Settings",meta = (AllowPrivateAccess = "true"))
	FCameraSetting BossBattleCameraSetting;
};
