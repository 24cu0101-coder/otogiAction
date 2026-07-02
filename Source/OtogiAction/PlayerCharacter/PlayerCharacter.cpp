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

	//キャラクター移動コンポーネント生成
	MovementCharaComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));

	//カメラ操作コンポーネント生成
	MovementCameraComp = CreateDefaultSubobject<UMoveCameraComponent>(TEXT("CameraComp"));

	//回避コンポーネント生成
	PlayerDodgeComp = CreateDefaultSubobject<UPlayerDodgeComponent>(TEXT("DodgeComp"));

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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCharacterMovement);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCameraMovement);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::OnPlayerDodge);

	}

}

//キャラクター移動
void APlayerCharacter::OnCharacterMovement(const FInputActionValue& Value)
{
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
	//スティックの傾きの軸を取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (MovementCameraComp)
	{
		//移動関数を呼び出す
		MovementCameraComp->CameraMove(LookAxisVector);
	}
}

//回避の処理(下記途中)
void APlayerCharacter::OnPlayerDodge(const FInputActionValue& Value)
{
	//componentがあったら
	if (PlayerDodgeComp)

	{
		PlayerDodgeComp->ExecuteAbility();
	}
		

}

