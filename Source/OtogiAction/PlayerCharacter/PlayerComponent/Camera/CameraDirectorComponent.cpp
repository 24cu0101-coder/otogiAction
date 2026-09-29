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

	m_PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	//NULLチェック
	if (!m_PlayerCharacter)return;

	m_MoveCameraComp = m_PlayerCharacter->GetCustomCameraComponent();

	m_SpringArmComp = m_PlayerCharacter->FindComponentByClass<USpringArmComponent>();

	m_CameraComp = m_PlayerCharacter->FindComponentByClass<UCameraComponent>();
	
}


// 毎フレーム呼ばれる
void UCameraDirectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bPlayingSequence)return;
	
	ApplyModifier(DeltaTime);
}

void UCameraDirectorComponent::SetModifier(UCameraModifierDataAsset* Modifier, float Weight)
{
	if (!Modifier)return;

	for (FActiveModifier& Active : m_ActiveModifier)
	{
		if(Active.m_Data == Modifier)
		{
			Active.Weight = Weight;
			return;
		}
		
	}
	FActiveModifier NewModifier;
	NewModifier.m_Data = Modifier;
	NewModifier.Weight = Weight;

	m_ActiveModifier.Add(NewModifier);
}


void UCameraDirectorComponent::ClearModifier(UCameraModifierDataAsset* Modifier)
{
	if (!Modifier)return;

	m_ActiveModifier.RemoveAll([Modifier](const FActiveModifier& Active) {return Active.m_Data == Modifier;});
}

void UCameraDirectorComponent::PlaySequence(ULevelSequence* Sequence)
{
	if (!Sequence || !m_PlayerCharacter)return;

	APlayerController* PC = Cast<APlayerController>(m_PlayerCharacter->GetController());

	if (!PC)return;

	FMovieSceneSequencePlaybackSettings Settings;

	Settings.bAutoPlay = true;

	ALevelSequenceActor* SequenceActor = nullptr;

	m_SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		Sequence,
		Settings,
		SequenceActor
	);

	if (!m_SequencePlayer) return;

	bPlayingSequence = true;

	m_SequencePlayer->OnFinished.AddDynamic(
		this,
		&UCameraDirectorComponent::StopSequence
	);

	m_SequencePlayer->Play();
}

void UCameraDirectorComponent::StopSequence()
{
	bPlayingSequence = false;

	m_SequencePlayer = nullptr;

	ReturnToGameplayCamra();
}

void UCameraDirectorComponent::ApplyModifier(float DeltaTime)
{
	if (!m_SpringArmComp || !m_CameraComp)return;
	
	float ArmLengthOffset = 0.f;
	FVector SocetOffset = FVector::ZeroVector;
	FRotator RotationOffset = FRotator::ZeroRotator;
	float FOVOffset = 0.f;

	for (const FActiveModifier& Active : m_ActiveModifier)
	{
		if (!Active.m_Data || Active.Weight <= 0.f)
		{
			continue;
		}
		const float Weight = Active.Weight;
		ArmLengthOffset += Active.m_Data->TargetArmLengthOffset * Weight;

		SocetOffset += Active.m_Data->SocketOffset * Weight;

		RotationOffset += Active.m_Data->RotationOffset * Weight;

		FOVOffset += Active.m_Data->FOVOffset * Weight;
	}

	const float TargetArmLength = 400.f + ArmLengthOffset;

	const FVector TargetSocketOffset = SocetOffset;

	const FRotator TargetRotationOffset = RotationOffset;

	const float TargetFOV = 90.0f + FOVOffset;

	m_SpringArmComp->TargetArmLength = FMath::FInterpTo(m_SpringArmComp->TargetArmLength, TargetArmLength, DeltaTime, 8.f);

	m_SpringArmComp->SocketOffset = FMath::VInterpTo(m_SpringArmComp->SocketOffset, TargetSocketOffset, DeltaTime, 8.0f);
	
	m_CameraComp->FieldOfView = FMath::FInterpTo(m_CameraComp->FieldOfView, TargetFOV, DeltaTime, 8.0f);
}

void UCameraDirectorComponent::ReturnToGameplayCamra()
{
}

