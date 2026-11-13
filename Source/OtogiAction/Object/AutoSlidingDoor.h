//担当：飯島
//ドアオブジェクトの開閉

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoSlidingDoor.generated.h"

//前方宣言
class UStaticMeshComponent;
class UBoxComponent;
class UPrimitiveComponent;

//ドアの開閉状態
UENUM(BlueprintType)
enum class EDoorState : uint8
{
	closed,
	opening,
	open,
	closing
};

UCLASS()
class OTOGIACTION_API AAutoSlidingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	//コンストラクタ
	AAutoSlidingDoor();

protected:
	//生成されたら処理
	virtual void BeginPlay() override;

	//毎フレーム処理
	virtual void Tick(float DeltaTime) override;

    // ルート
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<USceneComponent> SceneRoot;

    // 扉の外枠
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UStaticMeshComponent> DoorFrame;

    // 左側の扉
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UStaticMeshComponent> LeftDoor;

    // 右側の扉
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UStaticMeshComponent> RightDoor;

    // プレイヤー検知
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UBoxComponent> DetectionBox;

    // 左右それぞれの移動距離
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings",meta = (ClampMin = "0.0"))
    float OpenDistance = 150.0f;

    // 開閉にかかる時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Settings", meta = (ClampMin = "0.01"))
    float OpenDuration = 1.0f;

    // 扉の状態
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door|State")
    EDoorState DoorState = EDoorState::closed;

private:

    // 閉じた状態の座標
    FVector LeftClosedLocation;
    FVector RightClosedLocation;

    // 開いた状態の座標
    FVector LeftOpenLocation;
    FVector RightOpenLocation;

    // 0.0 = 閉じている、1.0 = 開いている
    float OpenAlpha = 0.0f;

    // 現在検知中のプレイヤー
    TSet<TWeakObjectPtr<AActor>> OverlappingPlayers;

    // 開閉更新
    void UpdateDoor(float DeltaTime);

    // プレイヤーが近づいた
    UFUNCTION()
    void OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);

    // プレイヤーが離れた
    UFUNCTION()
    void OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // 扉を開く
    void OpenDoor();

    // 扉を閉じる
    void CloseDoor();
};
