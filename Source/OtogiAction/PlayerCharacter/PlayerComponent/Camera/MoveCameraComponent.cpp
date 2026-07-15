//----------------------------------------
//カメラ操作を担うコンポーネント
//----------------------------------------

#include "MoveCameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "../PlayerTargetComponent.h"
#include "Camera/PlayerCameraManager.h"

//コンストラクタ
UMoveCameraComponent::UMoveCameraComponent()
{
	//Tick有効
	PrimaryComponentTick.bCanEverTick = true;
	PlayerTargetComp = nullptr;

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

	//ターゲットコンポーネントが存在し、かつロックオン中ならカメラを強制移動
	if (PlayerTargetComp && PlayerTargetComp->IsTargeting())
	{
		AActor* TargetActor = PlayerTargetComp->GetCurrentTargetActor();
		if (TargetActor && SpringArmComp)
		{
			// スプリングアームの現在の位置と、敵の位置を取得
			FVector CameraLoc = SpringArmComp->GetComponentLocation();
			FVector TargetLoc = TargetActor->GetActorLocation();

			// カメラから敵への回転角度を計算
			FRotator LookAtRot = FRotationMatrix::MakeFromX(TargetLoc - CameraLoc).Rotator();

			// 現在のカメラのワールド回転を取得
			FRotator CurrentRot = SpringArmComp->GetComponentRotation();

			// 補間
			FRotator SmoothRot = FMath::RInterpTo(CurrentRot, LookAtRot, DeltaTime, 5.0f);

			// ピッチの制限
			SmoothRot.Pitch = FMath::Clamp(SmoothRot.Pitch, -60.0f, 60.0f);
			SmoothRot.Roll = 0.0f; // ロールは傾かないように0固定

			// プリングアームの回転を直接上書き！
			SpringArmComp->SetWorldRotation(SmoothRot);
		}
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

