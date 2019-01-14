

#include "CTextWidget.h"

FText UCTextWidget::GetDisplayText() const
{
	return DisplayText;
}

void UCTextWidget::SetDisplayText(const FText & NewDisplayText)
{
	DisplayText = NewDisplayText;
	//UE_LOG(LogTemp, Warning, TEXT("VALUE %s"), *DisplayText.ToString());
}

void UCTextWidget::SetText(FText & Text, const FText & NewValue)
{
	Text = NewValue;
}
