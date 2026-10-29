// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalComboAttackComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/Move/MoveComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/InputBuffer/InputBufferComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


//コンストラクタ
UNormalComboAttackComponent::UNormalComboAttackComponent():
AttackWarp(false)

{
	PrimaryComponentTick.bCanEverTick = true;


	//キャラクター移動コンポーネント生成
	MCC = CreateDefaultSubobject<UMoveComponent>(TEXT("NAMC"));

	NAInputBufferComp = CreateDefaultSubobject<UInputBufferComponent>(TEXT("NormalAttacksInputBufferComp"));
}

void UNormalComboAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	PlayerActor = Cast<APlayerCharacter>(OwnerActor);
	AbilitySystemComponent = PlayerActor->GetAbilitySystemComponent();

	UCharacterMovementComponent* MoveComp = PlayerActor->GetCharacterMovement();
	if (MoveComp)
	{


		MoveComp->GravityScale = 100.0f;

		MoveComp->AirControl = 0.3f;
	}


	if (!AbilitySystemComponent)
	{
		return;
	}

	AttackCancel = false;


	//AbilitySystemComponent->InitAbilityActorInfo(GetOwner(), GetOwner());
	{
		if (GetOwner()->HasAuthority() && NAttackAbility1)
		{
			//プレイヤーにNAttackAbility1付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbility1, 1));
		}

		if (GetOwner()->HasAuthority() && NAttackAbility2)
		{
			//プレイヤーにNAttackAbility2付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbility2, 1));
		}

		if (GetOwner()->HasAuthority() && NAttackAbility3)
		{
			//プレイヤーにNAttackAbility3付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbility3, 1));
		}

		if (GetOwner()->HasAuthority() && NAttackAbility4)
		{
			//プレイヤーにNAttackAbility2付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbility4, 1));
		}

		if (GetOwner()->HasAuthority() && NAttackAbilityFinish)
		{
			//プレイヤーにNAttackAbility3付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(NAttackAbilityFinish, 1));
		}

		if (GetOwner()->HasAuthority() && DAttackAbility)
		{
			//プレイヤーにNAttackAbility3付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DAttackAbility, 1));
		}


		if (GetOwner()->HasAuthority() && KintaroAttack1)
		{
			//プレイヤーにNAttackAbility1付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(KintaroAttack1, 1));
		}

		if (GetOwner()->HasAuthority() && KintaroAttack2)
		{
			//プレイヤーにNAttackAbility2付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(KintaroAttack2, 1));
		}

		if (GetOwner()->HasAuthority() && KintaroAttack3)
		{
			//プレイヤーにNAttackAbility3付与
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(KintaroAttack3, 1));
		}

		SetAbilityTag();

	}
}


void UNormalComboAttackComponent::CancelAttackAbility()
{

	//FGameplayTag DontAttackTag = FGameplayTag::RequestGameplayTag(FName("PlayerNotify.CantAttack"));

	//if (!AbilitySystemComponent) return;

	if (AttackCancel)
	{
		FGameplayTagContainer AttackTags;
		AttackTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Attacking")));

		AbilitySystemComponent->CancelAbilities(&AttackTags);

		AttackCancel = false;
	}

}

void UNormalComboAttackComponent::SetAbilityTag()
{
	//タグ
//入力が可能かどうかのタグ
	PossibleNAttack = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.CantAttack"));

	//五段攻撃派生タグ
	NextTag0 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag0"));
	NextTag1 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag1"));
	NextTag2 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag2"));
	NextTag3 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag3"));
	NextTag4 = FGameplayTag::RequestGameplayTag(TEXT("PlayerNotify.NextTag4"));

	//ジャスト回避攻撃
	m_dodgeAttackTag = FGameplayTag::RequestGameplayTag(TEXT("IsDodge.DodgeAttack"));


	//金太郎タグ
	kintaroTag = FGameplayTag::RequestGameplayTag(TEXT("State.Skill.Kintaro"));

	SkillAttack = FGameplayTag::RequestGameplayTag(TEXT("State.Attacking.Skill"));
	//

}

//通常攻撃アビリティ実行(プレイヤーキャラクターで呼ばれる)
void UNormalComboAttackComponent::ExecuteNormalAttackAbility()
{
	if (NAInputBufferComp)
	{
		NAInputBufferComp->KeepOrExeFunction([this]()
			{
				ExectueNormalAttackAbility2();
			});
	}

}

void UNormalComboAttackComponent::ExectueNormalAttackAbility2()
{


	if (!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack) || AbilitySystemComponent->HasMatchingGameplayTag(SkillAttack))
	{

		return;
	}

	//ジャスト回避中のタグがあれば
	if (AbilitySystemComponent->HasMatchingGameplayTag(m_dodgeAttackTag))
	{
		if (DAttackAbility)
		{
			//アビリティ実行
			AbilitySystemComponent->TryActivateAbilityByClass(DAttackAbility);
			AbilitySystemComponent->RemoveLooseGameplayTag(m_dodgeAttackTag);
		}
	}
	
	else
	{



		//金太郎タグがあったら
		if (AbilitySystemComponent->HasMatchingGameplayTag(kintaroTag))
		{
			//アビリティがあるなら
			//現在のコンボ進行タグが2なら
			if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag2))
			{
				//アビリティがあるなら
				if (KintaroAttack3)
				{
					//アビリティ実行
					AbilitySystemComponent->TryActivateAbilityByClass(KintaroAttack3);
					AbilitySystemComponent->RemoveLooseGameplayTag(NextTag2);
				}
			}

			//現在のコンボ進行タグが1なら
			else if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag1))
			{
				//アビリティがあるなら
				if (KintaroAttack2)
				{
					//アビリティ実行
					AbilitySystemComponent->TryActivateAbilityByClass(KintaroAttack2);
					AbilitySystemComponent->RemoveLooseGameplayTag(NextTag1);
				}
			}

			//入力が可能だったら
			else if (!AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack))
			{
				MCC->StartWarping(600.f);


				AbilitySystemComponent->TryActivateAbilityByClass(KintaroAttack1);
			}
		}


		else
		{
			//現在のコンボ進行タグが2なら
			if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag4))
			{
				//アビリティがあるなら
				if (NAttackAbilityFinish)
				{
					//アビリティ実行as
					AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbilityFinish);
					AbilitySystemComponent->RemoveLooseGameplayTag(NextTag4);
				}
			}

			//現在のコンボ進行タグが2なら
			else if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag3))
			{
				//アビリティがあるなら
				if (NAttackAbility4)
				{
					//アビリティ実行as
					AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility4);
					AbilitySystemComponent->RemoveLooseGameplayTag(NextTag3);
				}
			}

			//現在のコンボ進行タグが2なら
			else if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag2))
			{
				//アビリティがあるなら
				if (NAttackAbility3)
				{
					//アビリティ実行as
					AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility3);
					AbilitySystemComponent->RemoveLooseGameplayTag(NextTag2);
				}
			}

			//現在のコンボ進行タグが1なら
			else if (AbilitySystemComponent->HasMatchingGameplayTag(NextTag1))
			{
				//アビリティがあるなら
				if (NAttackAbility2)
				{
					//アビリティ実行
					AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility2);
					AbilitySystemComponent->RemoveLooseGameplayTag(NextTag1);
				}
			}

			//else if(AbilitySystemComponent->HasMatchingGameplayTag(NextTag0))
			//{
			//	AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility1);
			//	AbilitySystemComponent->RemoveLooseGameplayTag(NextTag0);
			//}

			//入力が可能だったら
			else if (!AbilitySystemComponent->HasMatchingGameplayTag(PossibleNAttack))
			{
				AbilitySystemComponent->TryActivateAbilityByClass(NAttackAbility1);
			}
			if (MCC)
			{
				//AttackWarp = true;
				MCC->StartWarping(600.f);
			}
		}
	}
}

 
void UNormalComboAttackComponent::SetCancelAttackFlag()
{
	AttackCancel = true;
}

// 攻撃時の跳躍
void UNormalComboAttackComponent::AttackJump(float upPower)
{
	//if (!PlayerActor) return;

	//PlayerActor->LaunchCharacter(FVector(0.0f, 0.0f, upPower), false, true);
}

void UNormalComboAttackComponent::AttackMove(float forward, float upPower)
{
	if (!PlayerActor || AttackWarp) return;

	// プレイヤー正面のVector
	FVector ForwardVector = PlayerActor->GetActorForwardVector() * forward;
	
	if (upPower > 0)
	{
		ForwardVector.Z = upPower;
	}
	

	PlayerActor->LaunchCharacter(ForwardVector, false, true);


}
