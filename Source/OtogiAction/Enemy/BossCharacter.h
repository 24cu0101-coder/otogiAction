// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossChaseComponent.h"
#include "BossStrafeComponent.h"

#include "BossCharacter.generated.h"

UCLASS()
class OTOGIACTION_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//追跡処理のコンポーネント
	UPROPERTY(VisibleAnywhere) 
	TObjectPtr<UBossChaseComponent> ChaseComponent;

	//見合う処理のコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBossStrafeComponent> StrafeComponent;

	// TakeDamage をオーバーライド
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	//死亡時の処理
	void Die();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
