// スキルゲージを管理するコンポーネント

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillGaugeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API USkillGaugeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillGaugeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//スキル発動時にスキルゲージが足りているか確認する関数
	UFUNCTION(BlueprintCallable, Category = "SkillGauge")
	bool CanUseSkill(float SkillCost)const;

	//スキルを使用する際にゲージを減らす関数
	UFUNCTION(BlueprintCallable, Category = "SkillGauge")
	bool ConsumeGauge(float Amount);

	//ゲージを回復させる関数
	UFUNCTION(BlueprintCallable, Category = "SkillGauge")
	void ModifyGauge(float Amount);

	//ゲージを取得するための関数
	UFUNCTION(BlueprintCallable, Category = "SkillGauge")
	float GetGaugeRatio() const;

	//現在のゲージを取得
	FORCEINLINE float GetCurrentGauge() const { return CurrentSkillGauge; }

protected:
	//ゲージの最大値
	float MaxSkillGauge;

private:
	//現在のゲージ
	float CurrentSkillGauge;




};
