#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnDamaged,
	float,
	CurrentHP);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FOnDead);

class UNiagaraSystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UStatusComponent
 : public UActorComponent
{
	GENERATED_BODY()

public:

	UStatusComponent();

protected:

	virtual void BeginPlay() override;

public:

	// 最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHP = 100.f;

	// 現在HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float CurrentHP = 100.f;

	// 攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Attack = 20.f;

	// ダメージ通知
	UPROPERTY(BlueprintAssignable)
	FOnDamaged OnDamaged;

	// 死亡通知
	UPROPERTY(BlueprintAssignable)
	FOnDead OnDead;

	// ダメージ
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	// 回復
	UFUNCTION(BlueprintCallable)
	void Heal(float HealAmount);

	// 死亡判定
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	UNiagaraSystem* DarkParticleAsset;

private:
	//敵のパーティクル生成関数
	void SpawnDarkParticleOnDamage(FVector SpawnLocation);
};