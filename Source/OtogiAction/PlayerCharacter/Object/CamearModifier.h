//担当：飯島
//カメラの変化量を持つオブジェクト
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CamearModifier.generated.h"

//カメラの設定の変数軍
USTRUCT(BlueprintType)
struct FCameraModifierData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArmLengthOffset = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector SocketOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator RotationOffset = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FOVOffset = 0.0f;
};

UCLASS()
class OTOGIACTION_API UPlayerCamearModifier : public UObject
{
	GENERATED_BODY()

public:
    //カメラのModifier更新
    virtual void UpdateModifier(float DeltaTime, FCameraModifierData& ModifierData);

    //変更完了
    virtual bool IsFinish() const;

public:

    //変更にかかる速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Modifier")
    float Weight = 1.0f;

    //変更にかかる時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Modifier")
    float Duration = 0.0f;

protected:

    float ElapsedTime = 0.0f;

	
};
