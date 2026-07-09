//---------------------------------------
//キャラクターん歩行を担うコンポーネント
//---------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"

//前方宣言
class ACharacter;
class UPlayerTargetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//コンストラクタ
	UMoveComponent();

protected:
	//ゲームが始まったときか呼ばれたときに生成される
	virtual void BeginPlay() override;

public:	
	//毎フレーム処理される
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//characterの前後移動用関数
	void MoveForword(float Value);

	//characterの左右移動用関数
	void MoveRight(float Value);

	//キャラクターを指定座標まで移動させる関数
	void RotateActorToVector(FVector TargetLocation);

	//移動時の自動向き直り
	void SetOrientRotationToMovement(bool bEnable);

	// 入力量で速度を変更する関数
	void UpdateMovementSpeed(float InputRatio);

	//攻撃時に呼び出して敵に向かって吸い寄せを開始する関数
	void StartWarping(float FoftLockRadius);

protected:
	//歩きスピード
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float WalkSpeed = 300.0f;

	//ダッシュスピード
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float DashSpeed = 600.0f;

	//スティック倒したらダッシュにする場合の数値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float DashThreshold = 0.7f;

	UPROPERTY()
	UPlayerTargetComponent* PlayerTargetComp;

private:

	//このコンポーネントの持ち主
	UPROPERTY()
	ACharacter* OwnerCharacter;

	//吸い寄せ制御の変数
	bool bIsWarping = false;

	UPROPERTY()
	AActor* WarpTargetActor = nullptr;

};
