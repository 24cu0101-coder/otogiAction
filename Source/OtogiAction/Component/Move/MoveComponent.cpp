//---------------------------------------
//キャラクターん歩行を担うコンポーネント
//---------------------------------------

#include "MoveComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

//コンストラクタ
UMoveComponent::UMoveComponent()
{
	//Tick処理をオフにする
	PrimaryComponentTick.bCanEverTick = false;

}


// ゲームが始まったときによばれる
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	//持ち主のキャラクターを取得
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//移動初期設定
	//持ち主がいなければスルー
	if (OwnerCharacter)
	{
		//カメラに体を固定しない
		OwnerCharacter->bUseControllerRotationYaw = false;

		if (UCharacterMovementComponent* MoveComp = OwnerCharacter->GetCharacterMovement())
		{
			//移動方向にアクターを向かせる
			MoveComp->bOrientRotationToMovement = true;

			//アクターの回転スピード
			MoveComp->RotationRate = FRotator(0.f, 500.f, 0.f);

			//歩行スピード
			MoveComp->MaxWalkSpeed = WalkSpeed;
		}
	}
	
}


//毎フレーム処理
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 入力量で速度を変更する関数
void UMoveComponent::UpdateMovementSpeed(float InputRatio)
{
	if (!OwnerCharacter || !OwnerCharacter->GetCharacterMovement()) return;

	//スティックを倒して一定値を超えていたらダッシュ
	float TargetSpeed = (InputRatio >= DashThreshold) ? DashSpeed : WalkSpeed;

	//速度を設定
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
}


//キャラクターの前後移動
void UMoveComponent::MoveForword(float Value)
{
	if (!OwnerCharacter || !OwnerCharacter->GetController() || Value == 0.0f) return;

	//コントローラーの向き
	const FRotator Rotation = OwnerCharacter->GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	//カメラの前方ベクトル
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//移動入力を与える
	OwnerCharacter->AddMovementInput(Direction, Value);
}

//キャラクターの左右移動
void UMoveComponent::MoveRight(float Value)
{
	if (!OwnerCharacter || !OwnerCharacter->GetController() || Value == 0.0f) return;

	//コントローラーの向き
	const FRotator Rotation = OwnerCharacter->GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	//カメラの前方ベクトル
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//移動入力を与える
	OwnerCharacter->AddMovementInput(Direction, Value);
}

//キャラクターを指定座標まで移動させる関数
void UMoveComponent::RotateActorToVector(FVector TargetLocation)
{
	if (!OwnerCharacter || TargetLocation.IsZero()) return;

	//相手のアクターの角度を取得
	FVector MyLoc = OwnerCharacter->GetActorLocation();
	FVector Direction = TargetLocation - MyLoc;
	Direction.Z = 0.f;
	Direction.Normalize();

	FRotator TargetRot = Direction.Rotation();

	//振り向きを切る
	SetOrientRotationToMovement(false);

	//アクターを回転
	OwnerCharacter->SetActorRotation(TargetRot);
}

//移動時の自動向き直り
void UMoveComponent::SetOrientRotationToMovement(bool bEnable)
{
	if (OwnerCharacter && OwnerCharacter->GetCharacterMovement())
	{
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = bEnable;
	}
}

