// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundBase.h"
#include "CharacterAudioComponent.generated.h"


// 音の種類
UENUM(BlueprintType)
enum class ECharacterSoundType : uint8
{
	AttackNormal	UMETA(DisplayName = "Attack Normal"),
	AttackStrong	UMETA(DisplayName = "Attack Strong"),
	AttackSkill		UMETA(DisplayName = "Attack Skill"),

	Footstep		UMETA(DisplayName = "Footstep"),
	Jump			UMETA(DisplayName = "Jump"),
	Land			UMETA(DisplayName = "Land"),
	Dash			UMETA(DisplayName = "Dash"),

	Damage			UMETA(DisplayName = "Damage"),
	Guard			UMETA(DisplayName = "Guard"),
	Death			UMETA(DisplayName = "Death"),

	Voice			UMETA(DisplayName = "Voice")
};



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UCharacterAudioComponent : public UActorComponent
{
	GENERATED_BODY()


public:

	UCharacterAudioComponent();


	//指定した音を再生
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayCharacterSound(ECharacterSoundType SoundType);



protected:

	virtual void BeginPlay() override;



private:


	// 音データ管理
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio",
		meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterSoundType, USoundBase*> SoundList;



	// 音を実際に鳴らす処理
	void PlaySound(USoundBase* Sound);

};