// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "PlayerCharacter.generated.h"

//前方宣言
class UAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UMoveComponent;
class UMoveCameraComponent;
class UInputMappingContext;
class UInputAction;

class UPlayerDodgeComponent;		//回避を実行するクラス(髙山)
class UNormalAttackComponent;		//通常攻撃を実行するクラス(髙山)
class UPlayerTargetComponent;
class USkillComponent;
class UAttackCollisionComponent;
class UStatusComponent;
class UStrongAttackComponent;
class USkillGaugeComponent;
class UWeaponComponent;

UCLASS()
class OTOGIACTION_API APlayerCharacter : public ACharacter , public IAbilitySystemInterface
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

	//AbilitySystemInterfaceのゲッター関数
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//武器のクラスを外部に渡すゲッター関数
	UFUNCTION(BlueprintCallable, Category = "Components")
	UWeaponComponent* GetWeaponComponent() const { return WeaponComp; }

private:
	//スプリングアームコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_SpringArmComp;
	//カメラコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_CameraComp;

	//AbilitySystemコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComp;

	//キャラ移動コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UMoveComponent* MovementCharaComp;

	//カメラ操作コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UMoveCameraComponent* MovementCameraComp;

	//キャラ回避コンポーネント(髙山)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	UPlayerDodgeComponent* PlayerDodgeComp;

	//通常攻撃コンポーネント(髙山)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NormalAttack", meta = (AllowPrivateAccess = "true"))
	UNormalAttackComponent* NormalAttackComp;

	//強攻撃コンポーネント(髙山)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StrongAttack", meta = (AllowPrivateAccess = "true"))
	UStrongAttackComponent* StrongAttackComp;

	//スキルコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	USkillComponent* SkillComp;

	//ターゲットコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UPlayerTargetComponent* TargetComp;

	//ターゲットコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UAttackCollisionComponent* CollisionComp;

	//ステータスコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	UStatusComponent* StatusComp;

	//スキルゲージコンポネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	USkillGaugeComponent* GaugeComp;

	//武器こんぽーねんと
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWeaponComponent* WeaponComp;


	//-------------------------
	//Enhanced Input 設定
	//-------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	//回避のインプットアクション(髙山)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	//通常攻撃のインプットアクション(髙山)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAttackAction;

	//強攻撃のインプットアクション(髙山)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SAttackAction;


	//スキル選択
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchSkillGroup;

	//スキル発動
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ExcuteSkill1;

	//オーブ吸う
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AbsorbAction;

protected:
	//入力イベント発生時に実行される内部関数
	void OnCharacterMovement(const FInputActionValue& Value);
	void OnCameraMovement(const FInputActionValue& Value);
	void OnPlayerDodge(const FInputActionValue& Value);		//(髙山)
	void OnNormalAttack(const FInputActionValue& Value);	//(髙山)
	void OnStrongAttack();									//(髙山)

	TObjectPtr<UPlayerDodgeComponent>DodgeComponent;
	void OnSwitchSkillGroup(const FInputActionValue& Value);
	void OnSkill1Pressed();
	void OnSkill2Pressed();
	void OnSkill3Pressed();
	void OnSkill4Pressed();

	void OnAbsorb();
};
