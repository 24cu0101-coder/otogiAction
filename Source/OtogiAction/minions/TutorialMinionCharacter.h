// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "TutorialMinionCharacter.generated.h"


class UAnimMontage;

UCLASS()
class OTOGIACTION_API ATutorialMinionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATutorialMinionCharacter();

	// 攻撃アニメーション
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial Minion")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 攻撃を繰り返す間隔（秒）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial Minion")
	float AttackInterval = 3.0f;

	FTimerHandle AttackTimerHandle;

	virtual void BeginPlay() override;

	void PlayTutorialAttack();


	// 攻撃ダメージ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial Minion")
	float AttackDamage = 1.0f;

	// 攻撃判定の半径
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial Minion")
	float HitRadius = 220.0f;

	// 攻撃判定を前方に出す距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tutorial Minion")
	float HitDistance = 220.0f;


	// 攻撃判定を実行
	void ApplyTutorialAttackDamage();

protected:


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
