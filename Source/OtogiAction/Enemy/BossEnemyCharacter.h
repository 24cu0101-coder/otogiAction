// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BossEnemyCharacter.generated.h"


class UAbilitySystemComponent;
class UAttackCollisionComponent;

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
	//攻撃モンタージュの再生時間を返す関数
	float PlayAttackAnimation();

	//受けた攻撃分体力を減らし、現在HPを返す
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//敵のHP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Param")
	float EnemyHP;

	//敵の攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float AttackRange;


	//敵の攻撃判定の大きさ(幅、高さ、奥行)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float BoxWidth;		//幅
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float BoxHeight;	//高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float BoxLength;	//奥行
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float BoxOffSet;	//矩形をどれだけ前に出すか

	//攻撃アニメーションから攻撃判定の出現までのディレイ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float AttackDelay;

	//視野の範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float SightRange;
	//視野外に出る範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float LoseSightRange;
	//視野角の範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float SightDegrees;



protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	UAnimMontage* AttackMontage;

};
