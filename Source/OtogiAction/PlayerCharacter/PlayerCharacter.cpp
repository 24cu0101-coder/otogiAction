//-----------------------------
//プレイヤーの処理
//-----------------------------

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerComponent/Move/MoveComponent.h"
#include "PlayerComponent/Camera/MoveCameraComponent.h"
#include "PlayerComponent/PlayerDodgeComponent.h"
#include "PlayerComponent/SkillComponent.h"
#include "PlayerComponent/NormalAttack/NormalAttackComponent.h"
#include "PlayerComponent/PlayerTargetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "../Component/Status/StatusComponent.h"

//コンストラクタ
APlayerCharacter::APlayerCharacter()
{
 	//毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	//オフにする
	PrimaryActorTick.bCanEverTick = false;

	// デフォルトプレイヤーとして設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//スプリングアームとカメラの生成
	m_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	if (m_SpringArmComp)
	{
		//スプリングアームのアタッチ
		m_SpringArmComp->SetupAttachment(RootComponent);

		//スプリングアームの距離
		m_SpringArmComp->TargetArmLength = 400.f;

		//カメラの子コリジョンテストを行うか設定
		m_SpringArmComp->bDoCollisionTest = false;
		//カメラ追従ラグを使うか設定
		m_SpringArmComp->bEnableCameraLag = true;
		//カメラ追従ラグの速度を設定
		m_SpringArmComp->CameraLagSpeed = 1000.0f;
		//カメラ回転ラグを使うかを設定
		m_SpringArmComp->bEnableCameraRotationLag = true;
		//カメラ回転ラグの速度を設定
		m_SpringArmComp->CameraLagSpeed = 10.0f;
	}

	if ((m_CameraComp) && (m_SpringArmComp))
	{
		//カメラをスプリングアームにアタッチさせる
		m_CameraComp->SetupAttachment(m_SpringArmComp, USpringArmComponent::SocketName);
	}

	//AbilitySystem
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	//キャラクター移動コンポーネント生成
	MovementCharaComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));

	//カメラ操作コンポーネント生成
	MovementCameraComp = CreateDefaultSubobject<UMoveCameraComponent>(TEXT("CameraComp"));

	//回避コンポーネント生成
	PlayerDodgeComp = CreateDefaultSubobject<UPlayerDodgeComponent>(TEXT("DodgeComp"));


	//通常攻撃コンポーネント生成
	NormalAttackComp = CreateDefaultSubobject<UNormalAttackComponent>(TEXT("NormalAtComp"));

	//ターゲットコンポーネントの生成
	TargeComp = CreateDefaultSubobject<UPlayerTargetComponent>(TEXT("TargetComp"));

	//スキルコンポーネントの生成
	SkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComp"));

}

//ゲームが始まったときに生成
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Enhanced Input のマッピングコンテキストを追加
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//SkillComponentに自分のASCを渡す
	if (SkillComp && GetAbilitySystemComponent())
	{
		SkillComp->RegisterAbilities(GetAbilitySystemComponent());
	}

	if (MovementCharaComp && TargeComp)
	{
		//カメラコンポーネントにターゲットの参照を渡す
		MovementCameraComp->SetTargetComponent(TargeComp);
	}

	//ステータスコンポーネントから死亡をバインド
	UStatusComponent* StatusComp = FindComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		StatusComp->OnDead.AddDynamic(this, &APlayerCharacter::HandlePlayerDead);
	}

}

//毎フレーム処理
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//入力インプット
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 入力アクションをバインドする
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//キャラとカメラの移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCharacterMovement);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCameraMovement);;
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::OnPlayerDodge);
		EnhancedInputComponent->BindAction(NormalAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::OnNormalAttack);
		//回避
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCameraMovement);

		//ターゲットの入力
		EnhancedInputComponent->BindAction(TargetLockOn, ETriggerEvent::Started, this, &APlayerCharacter::OnTargetLockOn);
		//スキルの切り替え
		EnhancedInputComponent->BindAction(SwitchSkillGroup, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSwitchSkillGroup);
		//スキルの発動四つ
		EnhancedInputComponent->BindAction(ExcuteSkill1, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSkill1Pressed);
		EnhancedInputComponent->BindAction(ExcuteSkill2, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSkill2Pressed);
		EnhancedInputComponent->BindAction(ExcuteSkill3, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSkill3Pressed);
		EnhancedInputComponent->BindAction(ExcuteSkill4, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSkill4Pressed);

	}

}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

//キャラクター移動
void APlayerCharacter::OnCharacterMovement(const FInputActionValue& Value)
{
	if (bIsDead) return;

	//スティックの傾きの軸を取得
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (MovementCharaComp)
	{
		//傾きの角度を取得
		float CalculatedRatio = MovementVector.Size();

		//UpdateMovementSpeedに入力量を伝えて移動させる
		MovementCharaComp->UpdateMovementSpeed(CalculatedRatio);

		//移動処理を呼び出す
		MovementCharaComp->MoveForword(MovementVector.Y);
		MovementCharaComp->MoveRight(MovementVector.X);
	}
}

//カメラ操作
void APlayerCharacter::OnCameraMovement(const FInputActionValue& Value)
{
	if (bIsDead) return;

	//スティックの傾きの軸を取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (MovementCameraComp)
	{
		//移動関数を呼び出す
		MovementCameraComp->CameraMove(LookAxisVector);
	}
}

//回避(髙山)
void APlayerCharacter::OnPlayerDodge(const FInputActionValue& Value)
{
	if (bIsDead) return;
	
	//回避コンポーネントがあったら
	if (PlayerDodgeComp)
	{
		//コンポーネントの処理実行
		PlayerDodgeComp->ExecuteAbility();
	}
}       

//
void APlayerCharacter::OnNormalAttack(const FInputActionValue& Value) 
{
	if (bIsDead) return;
	//通常攻撃コンポーネントがあったら
	if (NormalAttackComp)
	{
		if (MovementCharaComp)
		{
			MovementCharaComp->StartWarping(600.f);
		}
		//コンポーネントの処理実行
		NormalAttackComp->ExecuteNormalAttackAbility();

	}
}
//ターゲットのロックオン
void APlayerCharacter::OnTargetLockOn()
{
	if (bIsDead) return;
	//コンポーネントがあるか確認
	if (TargeComp)
	{
		TargeComp->ToggleTargetLock();
	}
}
//スキル群の切り替え
void APlayerCharacter::OnSwitchSkillGroup(const FInputActionValue& Value)
{
	if (bIsDead) return;
	if (SkillComp)
	{
		//スキルの切り替え
		float AxsisValue = Value.Get<float>();
		int32 Direction = AxsisValue > 0.f ? 1 : -1;
		SkillComp->SwitchSkillGroup(Direction);
	}
}

//スキルの発動
void APlayerCharacter::OnSkill1Pressed()
{
	if (bIsDead) return;
	if (SkillComp)
	{
		SkillComp->RequestSkillTrigger(0);
	}
}
void APlayerCharacter::OnSkill2Pressed()
{
	if (bIsDead) return;
	if (SkillComp)
	{
		SkillComp->RequestSkillTrigger(1);
	}
}
void APlayerCharacter::OnSkill3Pressed()
{
	if (bIsDead) return;
	if (SkillComp)
	{
		SkillComp->RequestSkillTrigger(2);
	}
}
void APlayerCharacter::OnSkill4Pressed()
{
	if (bIsDead) return;
	if (SkillComp)
	{
		SkillComp->RequestSkillTrigger(3);
	}
}

//プレイヤーの死亡処理
void APlayerCharacter::HandlePlayerDead()
{
	//すでに死亡中なら返す
	if (bIsDead) return;
	bIsDead = true;

	UE_LOG(LogTemp, Warning, TEXT("[Player] Death Sequence Started."));

	// コントローラーからの入力を完全に遮断する
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	// 移動コンポーネントを完全に停止・無効化（慣性での滑りも止める）
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
	}

	//敵の攻撃や移動がすり抜けるように、カプセルコンポーネントの当たり判定を消す
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	//プレイヤー専用の死亡モンタージュを再生
	if (DeadMontage)
	{
		PlayAnimMontage(DeadMontage);
	}
	else
	{
		// もしモンタージュが設定されていなければ、安全のためにログを出す
		UE_LOG(LogTemp, Error, TEXT("[Player] DeathMontage is NOT set in Blueprint!"));
	}
}

