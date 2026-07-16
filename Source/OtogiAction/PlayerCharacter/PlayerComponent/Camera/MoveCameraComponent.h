//----------------------------------------
//カメラ操作を担うコンポーネント
//----------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveCameraComponent.generated.h"

//前方宣言
class ACharacter;
class USpringArmComponent;
class UPlayerTargetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UMoveCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//コンストラクタ
	UMoveCameraComponent();

	//プレイヤーのターゲットコンポーネントを外からセットしてあげるための関数
	void SetTargetComponent(class UPlayerTargetComponent* TargetComp) { PlayerTargetComp = TargetComp; }

protected:
	//ゲーム始まったときによばれる
	virtual void BeginPlay() override;

public:	
	//毎フレーム処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//プレイヤーの入力
	void CameraMove(FVector2D InputValue);

	//攻撃時に呼ぶことでカメラが暴れる事を防止するロック関数
	void SetCameraRotationLock(bool bLock);

	//カメラシェイクを外部で呼び出すための関数
	UFUNCTION(BlueprintCallable, Category = "Camera|Shake")
	void PlayerCameraShake(TSubclassOf<UCameraShakeBase>ShakeClass, float scale = 1.0f);
	
protected:
	//デフォルトのカメラシェイクを設定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Shake")
	TSubclassOf<UCameraShakeBase>DefaultCameraShakeClass;

private:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	//キャラクターが所持するスプイングアームを参照
	UPROPERTY()
	USpringArmComponent* SpringArmComp;

	//ターゲットコンポーネント
	UPROPERTY()
	UPlayerTargetComponent* PlayerTargetComp;
};
