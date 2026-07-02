//----------------------------------------
//カメラ操作を担うコンポーネント
//----------------------------------------

#include "MoveCameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

//コンストラクタ
UMoveCameraComponent::UMoveCameraComponent()
{
	//Tickはオフ
	PrimaryComponentTick.bCanEverTick = false;

}


//ゲームが始まったときに呼ばれる関数
void UMoveCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	//カメラを所持しているキャラクターを保持
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter)
	{
		//所持しているキャラクターからスプリングアームを追加
		SpringArmComp = OwnerCharacter->FindComponentByClass<USpringArmComponent>();

		
		if (SpringArmComp)
		{
			//カメラとアクターの同期を切る
			SpringArmComp->bUsePawnControlRotation = false;

			//アクターとのPitch回転の同期を切る
			SpringArmComp->bInheritPitch = false;

			//アクターとのYaw回転の同期を切る
			SpringArmComp->bInheritYaw = false;

			//アクターとのRoll回転の同期を切る
			SpringArmComp->bInheritRoll = false;

			//キャラクターのコントローラーの回転をカメラと同期
			OwnerCharacter->bUseControllerRotationYaw = false;
		}
	}
}


//毎フレーム処理する
void UMoveCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMoveCameraComponent::CameraMove(FVector2D InputValue)
{
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

