#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillGaugeComponent.generated.h"

// ゲージが変更されたときに通知するDelegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillGaugeChanged);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API USkillGaugeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USkillGaugeComponent();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;


	// スキル発動時にスキルゲージが足りているか確認
	UFUNCTION(BlueprintCallable, Category = "SkillGauge")
	bool CanUseSkill(float SkillCost) const;


	// スキル使用時にゲージを減らす
	UFUNCTION(BlueprintCallable, Category = "SkillGauge")
	bool ConsumeGauge(float Amount);


	// ゲージを回復
	UFUNCTION(BlueprintCallable, Category = "SkillGauge")
	void ModifyGauge(float Amount);


	// ゲージ割合を取得
	UFUNCTION(BlueprintCallable, Category = "SkillGauge")
	float GetGaugeRatio() const;


	// 現在のゲージを取得
	FORCEINLINE float GetCurrentGauge() const
	{
		return CurrentSkillGauge;
	}


	// ゲージが変更されたときに通知
	UPROPERTY(BlueprintAssignable, Category = "SkillGauge")
	FOnSkillGaugeChanged OnSkillGaugeChanged;


protected:

	// ゲージの最大値
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkillGauge")
	float MaxSkillGauge = 500.f;


private:

	// 現在のゲージ
	float CurrentSkillGauge;
};