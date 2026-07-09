#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "minionsAttackComponent.generated.h"

class APlayerCharacter;
class UStatusComponent;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EMinionsAttackType :uint8
{
	Normal,
	middle,
	Strong
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UminionsAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UminionsAttackComponent();

protected:

	virtual void BeginPlay() override;
public:
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebug = true;

private:
	float DebugTimer = 0.f;

	void DrawAttackRange();
public:
	// 攻撃タイプ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EMinionsAttackType AttackType = EMinionsAttackType::Normal;
	
	// 攻撃距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange = 200.f;

	// 攻撃可能か
	UFUNCTION(BlueprintCallable)
	bool CanAttack() const;

	// 攻撃
	UFUNCTION(BlueprintCallable)
	void Attack();

private:
	//ASC取得
	UAbilitySystemComponent* GetASC()const;

	// Player取得
	APlayerCharacter* GetPlayer() const;

	// 自分のStatus取得
	UStatusComponent* GetStatusComponent() const;
};