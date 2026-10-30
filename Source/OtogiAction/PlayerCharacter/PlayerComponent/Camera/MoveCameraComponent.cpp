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

}


//毎フレーム処理する
void UMoveCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMoveCameraComponent::CameraMove(FVector2D InputValue)
{
	if (!SpringArmComp)return;

	FRotator CurrentRot = SpringArmComp->GetComponentRotation();

	// 横回転
	CurrentRot.Yaw += InputValue.X;

	// 縦回転
	CurrentRot.Pitch = FMath::Clamp(CurrentRot.Pitch + InputValue.Y, -60.0f, 10.0f);

	// Rollは絶対に使わない
	CurrentRot.Roll = 0.0f;

	SpringArmComp->SetWorldRotation(CurrentRot);
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

