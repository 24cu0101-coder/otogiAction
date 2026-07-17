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

	//パンチアタックMontageの再生時間を返す関数
	float GetPlayPunchAttackMontageTime();
	//ジャンプアタックMontageの再生時間を返す関数
	float GetPlayJumpAttackMontageTime();

	//スピードをセットする関数
	void SetMovementSpeed(float NewSpeed);
	//スピードをゲットする関数
	float GetMovementSpeed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float HP;

	//敵の攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float AttackRange;

	//視野の範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float SightRange;
	//視野外に出る範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float LoseSightRange;
	//視野角の範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float SightDegrees;

	// AActorのTakeDamageをオーバーライド
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	//パンチアタックMontageの変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	UAnimMontage* PunchAttackMontage;

	//ジャンプアタックMontageの変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	UAnimMontage* JumpAttackMontage;

};
