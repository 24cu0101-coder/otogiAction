// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../Component/Audio/BGMManager.h"
#include "Sound/SoundBase.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (KintaroEffectComp)
	{
		// Deactivate() から DeactivateImmediate() に変更
		KintaroEffectComp->DeactivateImmediate();
	}

	// オーナーを取得
	if (AActor* Owner = GetOwner())
	{
		// オーナーからASC取得
		IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Owner);
		UAbilitySystemComponent* ASC = ASCInterface ? ASCInterface->GetAbilitySystemComponent() : nullptr;

		if (ASC)
		{
			FGameplayTag KintaroTag = FGameplayTag::RequestGameplayTag(TEXT("State.Skill.Kintaro"));

			// タグ変化で関数にバインド
			ASC->RegisterGameplayTagEvent(KintaroTag, EGameplayTagEventType::NewOrRemoved)
				.AddUObject(this, &UWeaponComponent::OnKintaroTagChanged);
		}
	}
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWeaponComponent::OnKintaroTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		//付与されたタイミングで金太郎に移行
		UE_LOG(LogTemp, Log, TEXT("KINTAROSTART"));
		SetWeaponStyle(EPlayerWeaponStyle::Kintaro);
	}
	else // NewCount == 0
	{
		//タグが消えたタイミングので、自動的に通常スタイルに戻る
		UE_LOG(LogTemp, Log, TEXT("KINTAROFFINIFH"));
		SetWeaponStyle(EPlayerWeaponStyle::Nomal);
	}
}

void UWeaponComponent::SetActorHidden(AActor* TargetActor, bool bNewHidden)
{
	if (!TargetActor) return;

	// 1. スタティックメッシュ（普通の刀）を探して非表示/表示
	TArray<UStaticMeshComponent*> StaticMeshes;
	TargetActor->GetComponents<UStaticMeshComponent>(StaticMeshes);
	for (UStaticMeshComponent* Mesh : StaticMeshes)
	{
		if (Mesh)
		{
			Mesh->SetHiddenInGame(bNewHidden);
		}
	}

	// 2. スケルタルメッシュ（骨入り刀）を探して非表示/表示
	TArray<USkeletalMeshComponent*> SkeletalMeshes;
	TargetActor->GetComponents<USkeletalMeshComponent>(SkeletalMeshes);
	for (USkeletalMeshComponent* Mesh : SkeletalMeshes)
	{
		if (Mesh)
		{
			Mesh->SetHiddenInGame(bNewHidden);
		}
	}
}

//武器の切り替えを行う関数
void UWeaponComponent::SetWeaponStyle(EPlayerWeaponStyle PlayerStyle)
{
	CurrentStyle = PlayerStyle;

	AActor* TargetWeaponActor = nullptr;
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	USoundManagerComponent* SoundMng = OwnerCharacter->FindComponentByClass<USoundManagerComponent>();

	if (!SoundMng)return;

	if (WeaponActorClass && OwnerCharacter)
	{
		TArray<AActor*> AttachedActors;
		OwnerCharacter->GetAttachedActors(AttachedActors);

		for (AActor* AttachedActor : AttachedActors)
		{
			if (AttachedActor && AttachedActor->IsA(WeaponActorClass))
			{
				TargetWeaponActor = AttachedActor;
				break;
			}
		}
	}

	// ★ここの一斉 Deactivate() は削除します

	// 現在のステートに合わせて「表示・非表示」と「エフェクトのON/OFF」を正しく切り替える
	switch (CurrentStyle)
	{
	case EPlayerWeaponStyle::Nomal:
		// 通常時は刀を表示して、エフェクトを消す
		if (TargetWeaponActor)
		{
			SetActorHidden(TargetWeaponActor, false);
		}
		if (KintaroEffectComp)
		{
			KintaroEffectComp->Deactivate();
		}
	
		//金太郎状態ループサウンドを停止
		if (m_KintaroAudioComponent)
		{
			SoundMng->StopLoopSound(m_KintaroAudioComponent);
			m_KintaroAudioComponent = nullptr;
		}

		
		break;

	case EPlayerWeaponStyle::Kintaro:
		// 金太郎時は刀を非表示にして、エフェクトを出す
		if (TargetWeaponActor)
		{
			SetActorHidden(TargetWeaponActor, true);
		}
		if (KintaroEffectComp)
		{
			KintaroEffectComp->Activate(true);
		}

		// 金太郎状態サウンド再生
		if (m_KintaroSound && !m_KintaroAudioComponent)
		{
			m_KintaroAudioComponent = SoundMng->PlayLoopSound(m_KintaroSound);
		}


		break;
	}
}

