// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

//前方宣言
class UAbilitySytemComponent;
class USpringArmComponent;
class UCameraComponent;
class UMoveComponent;
class UMoveCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPlayerDodgeComponent;		//回避を実行するクラス

UCLASS()
class OTOGIACTION_API APlayerCharacter : public ACharacter //, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	//コンストラクタ
	APlayerCharacter();

protected:
	//ゲームが始まったときに呼ばれる
	virtual void BeginPlay() override;

public:	
	//毎フレームよばれる
	virtual void Tick(float DeltaTime) override;

	//インプットバインド設定
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//外部からコンポーネントを触るため野ゲッター
	FORCEINLINE UMoveComponent* GetCustomMovementComponent() const { return MovementCharaComp; }
	FORCEINLINE UMoveCameraComponent* GetCustomCameraComponent() const { return MovementCameraComp; }

private:
	//スプリングアームコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_SpringArmComp;
	//カメラコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_CameraComp;

	//キャラ移動コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UMoveComponent* MovementCharaComp;

	//カメラ操作コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UMoveCameraComponent* MovementCameraComp;

	//Enhanced Input 設定
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;



protected:
	//入力イベント発生時に実行される内部関数
	void OnCharacterMovement(const FInputActionValue& Value);
	void OnCameraMovement(const FInputActionValue& Value);

	//回避のcomponentを登録(髙山記述)
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "DodgeComponent")
	TObjectPtr<UPlayerDodgeComponent>DodgeComponent;
};
