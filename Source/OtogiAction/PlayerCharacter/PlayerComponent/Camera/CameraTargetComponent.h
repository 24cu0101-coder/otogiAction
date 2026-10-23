//担当飯島
//カメラのターゲットを決めるコンポーネント

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraTargetComponent.generated.h"

//前方宣言
class UPlayerTargetComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UCameraTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//コンストラクタ
	UCameraTargetComponent();

	//生成されたときに処理される関数
	virtual void BeginPlay() override;

	//現在のロックオン対象を決める関数
	AActor* GetTargetActor() const;

	//通常時の中心点
	FVector GetDefaultTargetPoint() const;

	//ロックオン時の注視店
	FVector GetLockOnTargetPoint() const;

public:

	// プレイヤーを見る高さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Target")
	float PlayerFocusHeight = 80.0f;

	// 敵を見る高さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Target")
	float EnemyFocusHeight = 80.0f;

	// 敵側にどれくらい寄せるか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Target",meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float EnemyWeight = 0.65f;

private:

	UPROPERTY()
	TObjectPtr<UPlayerTargetComponent> PlayerTargetComponent;
};
