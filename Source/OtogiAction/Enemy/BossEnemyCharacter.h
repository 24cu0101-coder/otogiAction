// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BossEnemyCharacter.generated.h"


class UBossEnemyHitReactionComponent;
class UUHitReactionBaseComponent;

//ジャンプアタックMontageでNotifyが作動したときのdelegate
DECLARE_MULTICAST_DELEGATE(FOnJumpAttackNotifyDelegate);

UCLASS()
class OTOGIACTION_API ABossEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemyCharacter();

	// AActorのTakeDamageをオーバーライド
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//パンチアタックMontageの再生時間を返す関数
	float GetPlayPunchAttackMontageTime();
	//ジャンプアタックMontageの再生時間を返す関数
	float GetPlayJumpAttackMontageTime();

	//スピードをセットする関数
	void SetMovementSpeed(float NewSpeed);
	//スピードをゲットする関数
	float GetMovementSpeed()const;

	//被弾しているかどうかを返す関数
	bool GetIsHitFlg()const;
	//被弾のフラグをセットする関数
	void SetIsHitFlg(bool NewFlg);

	//回転する関数
	void RotateTowardsPlayer(float DeltaTime);

	//JumpAttackNotifyから呼び出す関数
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void TriggerJumpAttack();


	//敵の最大HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float MaxHP;
	//敵の現在HP
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AI|Param")
	float CurrentHP;



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

	//敵が被弾したかどうか
	bool IsHit = false;

	//被ダメージコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitReaction", meta = (AllowPrivateAccess = "true"))
	UUHitReactionBaseComponent* HitReactionComp;

	//JumpAttackTaskからバインドするための公開デリゲート
	FOnJumpAttackNotifyDelegate OnJumpAttackNotify;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	//パンチアタックMontageの変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	UAnimMontage* PunchAttackMontage;

	//ジャンプアタックMontageの変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack")
	UAnimMontage* JumpAttackMontage;

};
