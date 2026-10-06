//担当：飯島

//----------------------------------------
//カメラ操作を担うコンポーネント
//----------------------------------------

#include "MoveCameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../PlayerTargetComponent.h"
#include "Camera/PlayerCameraManager.h"

//コンストラクタ
UMoveCameraComponent::UMoveCameraComponent()
{
	//Tick有効
	PrimaryComponentTick.bCanEverTick = true;

	//ポインターの初期化
	PlayerTargetComp = nullptr;
	OwnerCharacter = nullptr;
	SpringArmComp = nullptr;
	CameraComp = nullptr;
}


//ゲームが始まったときに呼ばれる関数
void UMoveCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	//カメラを所持しているキャラクターをキャストして取得
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//キャラクターがいなかったら終了
	if (!OwnerCharacter) return;

	//SpringArmのポインター取得
	SpringArmComp = OwnerCharacter->FindComponentByClass < USpringArmComponent>();

	//カメラのポインター取得
	CameraComp = OwnerCharacter->FindComponentByClass<UCameraComponent>();

	//springarm設定
	if (SpringArmComp)
	{
		//カメラとアクターの同期を切る
		SpringArmComp->bUsePawnControlRotation = false;

		//Pitch
		SpringArmComp->bInheritPitch = false;

		//Yaw
		SpringArmComp->bInheritYaw = false;

		//Roll
		SpringArmComp->bInheritRoll = false;

		//キャラクター側のコントローラーの回転を切る
		OwnerCharacter->bUseControllerRotationYaw = false;
	}
}


//毎フレーム処理する
void UMoveCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerTargetComp && PlayerTargetComp->IsTargeting())
	{
		AActor* TargetActor = PlayerTargetComp->GetCurrentTargetActor();

		if (!TargetActor || !OwnerCharacter || !SpringArmComp || !CameraComp) return;
		
		//プレイヤーの注視点
		FVector PlayerPoint = OwnerCharacter->GetActorLocation();

		PlayerPoint.Z += PlayerTargetPointHeight;

		//敵の注視点
		FVector EnemyPoint = TargetActor->GetActorLocation();

		EnemyPoint.Z += EnemyTargetPointHeight;

		//プレイヤーと敵のポイントを結んだ中心点を求める
		FVector CameraTargetPoint = (PlayerPoint + EnemyPoint) * 0.5f;

		//実際のカメラ位置を取得
		FVector CameraLocation = CameraComp->GetComponentLocation();

		//カメラのとらえる中心点の方向を求める
		FVector LookDirection = CameraTargetPoint - CameraLocation;

		if (LookDirection.IsNearlyZero()) return;

		//ターゲット中のカメラの回転
		FRotator LookAtRotation = LookDirection.Rotation();

		//SpringArm回転
		FRotator CurrentRotation = SpringArmComp->GetComponentRotation();

		//回転の補間
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, DeltaTime, TargetCameraInterpSpeed);

		//上下のカメラ制限
		SmoothRotation.Pitch = FMath::Clamp(SmoothRotation.Pitch, MinTargetCameraPitch, MaxTargetCameraPitch); 

		//SpringArmを回転
		SpringArmComp->SetWorldRotation(SmoothRotation);
	}
}

void UMoveCameraComponent::CameraMove(FVector2D InputValue)
{
	//ロックオン中は右スティックでの手動カメラ操作を無効化する
	if (PlayerTargetComp && PlayerTargetComp->IsTargeting()) return;

	if (!OwnerCharacter || !OwnerCharacter->GetController()) return;

	if (SpringArmComp)
	{
		//世界の回転を取得
		FRotator CurrentRot = SpringArmComp->GetComponentRotation();

		// 画面がひっくり返らないように制限
		CurrentRot.Yaw += InputValue.X;
		CurrentRot.Pitch = FMath::Clamp(CurrentRot.Pitch + InputValue.Y, -60.0f, 60.0f); 

		//世界の角度でカメラの向きを上書き
		SpringArmComp->SetWorldRotation(CurrentRot);
	}
}

void UMoveCameraComponent::SetCameraRotationLock(bool bLock)
{
}

//カメラシェイク開始関数
//引き数で名前指定すればそのカメラシェイク出せるのと、ヌルにしといてもデフォルト設定してる物が出るよ
void UMoveCameraComponent::PlayerCameraShake(TSubclassOf<UCameraShakeBase> ShakeClass, float scale)
{
	if (!OwnerCharacter)return;

	if (APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController()))
	{
		if (PC->PlayerCameraManager)
		{
			//カメラシェイクをBPで設定してればそのまま実行
			TSubclassOf<UCameraShakeBase> ActiveShakeClass = ShakeClass ? ShakeClass : DefaultCameraShakeClass;

			if (ActiveShakeClass)
			{
				// カメラマネージャーにシェイクの開始を命令！
				PC->PlayerCameraManager->StartCameraShake(ActiveShakeClass, scale);
			}
		}
	}
}

