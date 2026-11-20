#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LongRangeMinion.generated.h"

class AArrowActor;
class USceneComponent;
class UAnimMontage;
class UWidgetComponent;
class UEnemyHPWidget;

UCLASS()
class OTOGIACTION_API ALongRangeMinion : public ACharacter
{
	GENERATED_BODY()

public:
	ALongRangeMinion();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,	class AController* EventInstigator,AActor* DamageCauser) override;

	// AnimNotify（FireArrow）から呼び出す矢の発射処理
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SpawnArrow();

protected:
	// HPバー
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComp;

	UPROPERTY()
	TObjectPtr<UEnemyHPWidget> HealthBarWidget;

	void UpdateHealthBar(float HealthPercent);

	// 発射位置（ソケット未指定時のフォールバック）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> MuzzlePoint;

	// 発射ソケット名（手や弓のソケットを指定）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName MuzzleSocketName = TEXT("None");

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AArrowActor> ArrowClass;

	// 1. 弓を構えるモンタージュ
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> PrepareMontage;

	// 2. 狙いを定めるモンタージュ
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> AimMontage;

	// 3. 矢を射るモンタージュ（中に FireArrow の AnimNotify を配置）
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 狙いを定める時間（秒）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AimDuration = 1.0f;

	// 状態フラグ
	bool bIsPreparing = false;
	bool bIsAiming = false;
	bool bIsAttacking = false;

	// 各段階の遷移処理
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FinishPrepare();

	void FinishAim();
	void StartAttackMontage();
	void FinishAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float FireInterval = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RotationInterpSpeed = 5.0f;

	// ステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float CurrentHealth = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float DestroyDelay = 3.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Status")
	void OnDeath();

	// 手に持たせる武器のメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	// 武器をアタッチするスケルトンのソケット
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName WeaponSocketName = TEXT("ArrowSocket");


private:
	FTimerHandle FireTimerHandle;
	FTimerHandle PrepareTimerHandle;
	FTimerHandle AimTimerHandle;
	FTimerHandle AttackTimerHandle;

	void StartAttack();
	void Die();
};