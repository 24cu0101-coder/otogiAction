// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "BossAIController.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//使用する AIController のクラスを指定
	AIControllerClass = ABossAIController::StaticClass();

	//自動的にPossessされるタイミングを設定
	// EAutoPossessAI::PlacedInWorldOrSpawned : レベル配置時・Spawn時のどちらでも自動生成＆Possess
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//コンポーネントのアタッチ
	ChaseComponent = CreateDefaultSubobject<UBossChaseComponent>(TEXT("ChaseComponent"));
	StrafeComponent = CreateDefaultSubobject<UBossStrafeComponent>(TEXT("StrafeComponent"));

}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//被弾時の処理
float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

//死亡時の処理
void ABossCharacter::Die()
{
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

