#include "PlayerHPWidget.h"
#include "Components/ProgressBar.h"

void UPlayerHPWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerHPWidget::SetHPPercent(float Percent)
{
	if (HPBar)
	{
		HPBar->SetPercent(Percent);
	}
}