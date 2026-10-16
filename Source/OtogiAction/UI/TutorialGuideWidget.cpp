#include "TutorialGuideWidget.h"
#include "Components/Image.h"

void UTutorialGuideWidget::SetTutorialImage(
    UTexture2D* NewTexture)
{
    if (TutorialImage && NewTexture)
    {
        TutorialImage->SetBrushFromTexture(NewTexture);
    }
}