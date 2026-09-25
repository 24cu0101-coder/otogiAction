//担当：飯島


#include "CameraDirectorComponent.h"
#include "../../DataAsset/Camera/Modifier/CameraModifierDataAsset.h"
#include "MoveCameraComponent.h"
#include "../../PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"


//コンストラクタ
UCameraDirectorComponent::UCameraDirectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


//ゲームが始まった時に呼ばれる処理
void UCameraDirectorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// 毎フレーム呼ばれる
void UCameraDirectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCameraDirectorComponent::SetModifier(UCameraModifierDataAsset* Modifier, float Weight)
{
}

void UCameraDirectorComponent::ClearModifier(UCameraModifierDataAsset* Modifier)
{
}

void UCameraDirectorComponent::PlaySequence(ULevelSequence* Sequence)
{
}

void UCameraDirectorComponent::StopSequence()
{
}

void UCameraDirectorComponent::ApplyModifier(float DeltaTime)
{
}

void UCameraDirectorComponent::ReturnToGameplayCamra()
{
}

