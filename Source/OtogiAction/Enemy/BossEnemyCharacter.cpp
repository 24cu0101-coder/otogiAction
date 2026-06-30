// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

// Sets default values
ABossEnemyCharacter::ABossEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;				//移動方向に向く
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);	//旋回速度（度/秒）
	}

}

// Called when the game starts or when spawned
void ABossEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float ABossEnemyCharacter::PlayAttackAnimation()
{
	if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		//Montageの再生時間を秒数で返す
		return PlayAnimMontage(AttackMontage);
	}
	return 0.f;
}

float ABossEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//体力を減らす
	EnemyHP -= ActualDamage;

	return EnemyHP;
}

// Called every frame
void ABossEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

