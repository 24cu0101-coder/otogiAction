#include "SkillGaugeWidget.h"
#include "Components/ProgressBar.h"

void USkillGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillGaugeWidget::SetGaugePercent(float Percent)
{
	if (GaugeBar)
	{
		GaugeBar->SetPercent(Percent);
	}
}