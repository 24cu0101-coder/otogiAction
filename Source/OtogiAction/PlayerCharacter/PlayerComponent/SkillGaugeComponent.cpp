// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillGaugeComponent.h"

// Sets default values for this component's properties
USkillGaugeComponent::USkillGaugeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//現在のゲージの初期化
	CurrentSkillGauge = 0.f;
}


// Called when the game starts
void USkillGaugeComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void USkillGaugeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USkillGaugeComponent::CanUseSkill(float SkillCost) const
{
	return CurrentSkillGauge >= SkillCost;
}

bool USkillGaugeComponent::ConsumeGauge(float Amount)
{
	//使用可能か確認
	if (!CanUseSkill(Amount))
	{
		return false;
	}

	CurrentSkillGauge = FMath::Clamp(CurrentSkillGauge - Amount, 0.f, MaxSkillGauge);
	return true;
}

void USkillGaugeComponent::ModifyGauge(float Amount)
{
	CurrentSkillGauge = FMath::Clamp(CurrentSkillGauge + Amount, 0.f, MaxSkillGauge);
}

float USkillGaugeComponent::GetGaugeRatio() const
{
	if (MaxSkillGauge <= 0.f)return 0.f;
	return CurrentSkillGauge / MaxSkillGauge;
}

