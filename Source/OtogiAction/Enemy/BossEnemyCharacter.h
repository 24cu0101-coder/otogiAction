// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BossEnemyCharacter.generated.h"


class UAbilitySystemComponent;

UCLASS()
class OTOGIACTION_API ABossEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemyCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//UŒ‚ƒ‚ƒ“ƒ^[ƒWƒ…‚ÌÄ¶ŠÔ‚ğ•Ô‚·ŠÖ”
	float PlayAttackAnimation();

	//ó‚¯‚½UŒ‚•ª‘Ì—Í‚ğŒ¸‚ç‚µAŒ»İHP‚ğ•Ô‚·
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//“G‚ÌHP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Param")
	float EnemyHP;

	//“G‚ÌUŒ‚”ÍˆÍ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float AttackRange;



	//‹–ì‚Ì”ÍˆÍ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float SightRange;
	//‹–ìŠO‚Éo‚é”ÍˆÍ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float LoseSightRange;
	//‹–ìŠp‚Ì”ÍˆÍ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float SightDegrees;

	//GAS‚ğg‚¤‚½‚ß‚Ì•Ï”
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	UAnimMontage* AttackMontage;

};
