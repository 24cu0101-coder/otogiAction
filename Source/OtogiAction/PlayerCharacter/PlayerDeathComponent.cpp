// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDeathComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "../OrtogiGameMode.h"

// Sets default values for this component's properties
UPlayerDeathComponent::UPlayerDeathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPlayerDeathComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerDeathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//死亡処理
void UPlayerDeathComponent::Dead()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Stop(0.1f);
	}


	//コリジョンを無効化
	/*if (Character->GetCapsuleComponent())
	{
		Character->GetCapsuleComponent()->SetCollisionResponseToChannels(ECC_Pawn, ECR_Ignore);
	}*/

	//死亡モンタージュ再生
	if (DeathMontage)
	{
		Character->PlayAnimMontage(DeathMontage);
	}
	else if (Character->GetMesh())
	{
		//ぐにゃらせる
		Character->GetMesh()->SetSimulatePhysics(true);
		Character->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	}

	//リスポーン実行タイマーをスタート
	FTimerHandle RespawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		RespawnTimerHandle,
		this,
		&UPlayerDeathComponent::RespawnTimerStart,
		RespawnDelay,
		false
	);

	
}


//リスポーン開始タイマー
void UPlayerDeathComponent::RespawnTimerStart()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	AController* PC = Character->GetController();

	/* //GameMode を取得してリスポーンを依頼
	if (AOtogiGameMode* GM = Cast<AOtogiGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->RespawnPlayer(PC);
	}*/
}