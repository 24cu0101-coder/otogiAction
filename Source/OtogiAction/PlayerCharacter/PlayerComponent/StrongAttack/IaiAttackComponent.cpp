// Fill out your copyright notice in the Description page of Project Settings.


#include "IaiAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "../Move/MoveComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/InputBuffer/InputBufferComponent.h"


UIaiAttackComponent::UIaiAttackComponent()
{
	SecondInputExecuted = false;

	PrimaryComponentTick.bCanEverTick = true;

	MCC = CreateDefaultSubobject<UMoveComponent>(TEXT("SAMC"));

	IaiAttackInputBufferComp = CreateDefaultSubobject<UInputBufferComponent>(TEXT("IaiAttacksInputBufferComp"));

}


// Called when the game starts
void UIaiAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	PlayerActor = Cast<APlayerCharacter>(OwnerActor);

	if (PlayerActor)
	{
		IaiAttackASC = PlayerActor->GetAbilitySystemComponent();
	}

	//強攻撃のアビリティがあれば
	if (IaiAttackAbility && IaiAttackASC)
	{
		if (GetOwner()->HasAuthority())
		{
			//プレイヤーにSAttackAbilityを付与する
			IaiHandle = IaiAttackASC->GiveAbility(FGameplayAbilitySpec(IaiAttackAbility, 1));
		}

		IaiAttackASC->OnAbilityEnded.RemoveAll(this);
		IaiAttackASC->OnAbilityEnded.AddUObject(this, &UIaiAttackComponent::AbilityEnd);
	}

	Iaidebug();
}

void UIaiAttackComponent::ExecuteIaiAttackAbility()
{
	if (IaiAttackInputBufferComp)
	{
		IaiAttackInputBufferComp->KeepOrExeFunction([this]()
			{
				ExecuteIaiAttackAbility2();
			});
	}

}


void UIaiAttackComponent::ExecuteIaiAttackAbility2()
{

	UE_LOG(LogTemp, Warning, TEXT("【IaiAttackComponent】eeeee"));

	if (!IaiAttackASC || !IaiAttackAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("【IaiAttackComponent】eeeee"));


		return;
	}

	// アビリティの状態情報を取得
	FGameplayAbilitySpec* Spec = IaiAttackASC->FindAbilitySpecFromHandle(IaiHandle);

	if (!Spec)
	{
		UE_LOG(LogTemp, Warning, TEXT("【IaiAttackComponent】uuuuuu"));
		return;
	}

	// アビリティが実行中かどうか
	if (Spec && Spec->IsActive())
	{
		// 二度目の入力でない場合
		if (!SecondInputExecuted)
		{
			SecondInputExecuted = true;
			UE_LOG(LogTemp, Warning, TEXT("【IaiAttackComponent】;;;;;"));

			FGameplayTag SecontInputTag = FGameplayTag::RequestGameplayTag(FName("Iai.SecondInput"));
			FGameplayEventData EventData;
			IaiAttackASC->HandleGameplayEvent(SecontInputTag, &EventData);
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("【IaiAttackComponent】fffff"));

		}
		return;
	}

	FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge"));

	FGameplayTag SAttackTag = FGameplayTag::RequestGameplayTag(FName("PlayerNotify.CantAttack"));


	if (IaiAttackASC->HasMatchingGameplayTag(DodgeTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("[IaiComp] 発動失敗: IsDodge タグが付与されています"));
		return;
	}

	if (IaiAttackASC->HasMatchingGameplayTag(SAttackTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("[IaiComp] 発動失敗: PlayerNotify.CantAttack タグが付与されています"));
		return;
	}
	////アビリティシステムコンポーネントがあり、回避と攻撃が実装中じゃなければ
	//if (IaiAttackASC->HasMatchingGameplayTag(DodgeTag) || IaiAttackASC->HasMatchingGameplayTag(SAttackTag))
	//{
	//	return;
	//}
	//
	SecondInputExecuted = false;
	bool bActivated = IaiAttackASC->TryActivateAbilityByClass(IaiAttackAbility);

	UE_LOG(LogTemp, Warning, TEXT("[IaiComp] 1回目の発動試行結果: %s"), bActivated ? TEXT("SUCCESS") : TEXT("FAILED"));
	
}

// アビリティの終わりを検知する
void UIaiAttackComponent::AbilityEnd(const FAbilityEndedData& AbilityEndedData)
{
	// 居合アビリティ時にフラグリセット
	if (AbilityEndedData.AbilitySpecHandle== IaiHandle)
	{
		SecondInputExecuted = false;
	}
}

void UIaiAttackComponent::Iaidebug()
{
	if (!PlayerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("【IaiAttackComponent】NotPlayActor"));
	}
	if (!IaiAttackAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("【IaiAttackComponent】NotIaiAttackAbility"));
	}
	if (!IaiAttackASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("【IaiAttackComponent】NotIaiAttackASC"));
	}	
}

