
#include "TutorialGuideWidget.h"
#include "Components/TextBlock.h"

void UTutorialGuideWidget::SetTutorialText(const FText& NewText)
{
    if (TutorialText)
    {
        TutorialText->SetText(NewText);
    }
}
