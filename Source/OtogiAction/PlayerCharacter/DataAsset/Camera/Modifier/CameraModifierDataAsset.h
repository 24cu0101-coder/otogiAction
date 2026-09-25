// 担当：飯島
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CameraModifierDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class OTOGIACTION_API UCameraModifierDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	//SpringArmの長さへの加算
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|SpringArm")
	float TargetArmLengthOffset = 0.0f;


	//SpringArmのソケットオフセット
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|SpringArm")
	FVector SocketOffset = FVector::ZeroVector;

	
	//SpringArmの回転offset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|SpringArm")
	FRotator RotationOffset = FRotator::ZeroRotator;

	//FOVの加算
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Lens")
	float FOVOffset = 0.0f;

	//適応速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float InterpSpeed = 5.0f;
};
