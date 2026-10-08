//担当：飯島
//アクション中に切り替わるカメラの値を保存しておくデータアセット

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionCameraSettingDataAsset.generated.h"


UCLASS()
class OTOGIACTION_API UActionCameraSettingDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//CamraDistance
	//SpringArmのアームの長さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Distance")
	float TargetArmLength = 400.0f;

	//敵との距離によってどれだけカメラを引くか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Distance")
	float DistanceScale = 0.15f;

	//アームの最小距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Distance")
	float MinArmLength = 300.0f;

	//アームの最長距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Distance")
	float MaxArmLength = 550.0f;


	//ターゲット時に使用するカメラフォーカス
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Focus", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float TargetFocusWeight = 0.65f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Focus")
	float PlayerFocusHeight = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Focus")
	float EnemyFocusHeight = 80.0f;

	//カメラの使う回転オフセット
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Offset")
	FVector SocketOffset = FVector::ZeroVector;

	//視野角
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Lens")
	float FOV = 90.0f;

	//適応スピード
	//回転適応スピード
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Interpolation")
	float RotationInterpSpeed = 8.0f;

	//距離適応スピード
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Interpolation")
	float DistanceInterpSpeed = 8.0f;

	//視野角適応スピード
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Interpolation")
	float FOVInterpSpeed = 8.0f;
};
