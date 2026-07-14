#include "EnemyHPWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHPWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemyHPWidget::SetHP(float CurrentHP, float MaxHP)
{
	if (HPBar && MaxHP > 0.f)
	{
		HPBar->SetPercent(CurrentHP / MaxHP);
	}
}