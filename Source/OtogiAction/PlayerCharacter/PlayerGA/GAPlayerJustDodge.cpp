// Fill out your copyright notice in the Description page of Project Settings.


#include "GAPlayerJustDodge.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "OtogiAction/GameFlow/Actor/TutorialActor.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"

UGAPlayerJustDodge::UGAPlayerJustDodge()
{

}

void UGAPlayerJustDodge::ActivateAbility(
	const FGameplayAbilitySpecHandle Dodging,
	const FGameplayAbilityActorInfo* playerActorInfo, 
	const FGameplayAbilityActivationInfo AvtivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
)
{
	Super::ActivateAbility(Dodging, playerActorInfo, AvtivationInfo, DodgeTriggerEvent);

	//アビリティ取得
	//ASC = GetAbilitySystemComponentFromActorInfo();

	////プレイヤーの情報を取得
	//PlayerActor = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	//if (!PlayerActor || !JustDodgeMontage)
	//{
	//	//リターン
	//	return;
	//}

	////タグ登録
	//IsDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.NormalDodge"));


	//StickRotate();


	////回避開始
	//DodgeStart();
}


//--------------------------------------------------------------------------------------------------------
//関数
//-----------------------------------------------------------------------------------------------------







