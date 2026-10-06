#include "SkillGaugeComponent.h"


USkillGaugeComponent::USkillGaugeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentSkillGauge = 0.f;
}


void USkillGaugeComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentSkillGauge = 0.f;
}


void USkillGaugeComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(
		DeltaTime,
		TickType,
		ThisTickFunction);
}


// スキルが使えるか確認
bool USkillGaugeComponent::CanUseSkill(float SkillCost) const
{
	return CurrentSkillGauge >= SkillCost;
}


// スキル使用時にゲージを減らす
bool USkillGaugeComponent::ConsumeGauge(float Amount)
{

	// 使用可能か確認
	if (!CanUseSkill(Amount))
	{
		return false;
	}

	// ゲージを減らす
	CurrentSkillGauge = FMath::Clamp(
		CurrentSkillGauge - Amount,
		0.f,
		MaxSkillGauge
	);



	// ゲージが変更されたことを通知
	OnSkillGaugeChanged.Broadcast();

	return true;
}


// ゲージを回復
void USkillGaugeComponent::ModifyGauge(float Amount)
{
	CurrentSkillGauge = FMath::Clamp(
		CurrentSkillGauge + Amount,
		0.f,
		MaxSkillGauge
	);

	// ゲージが変更されたことを通知
	OnSkillGaugeChanged.Broadcast();
}


// ゲージ割合を取得
float USkillGaugeComponent::GetGaugeRatio() const
{
	if (MaxSkillGauge <= 0.f)
	{
		return 0.f;
	}

	return CurrentSkillGauge / MaxSkillGauge;
}