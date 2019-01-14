/*Reference: https://answers.unrealengine.com/questions/513675/acces-text-from-umg-userwidget-from-c.html*/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class PLANTTHATWHEAT_API UCTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FText DisplayText;

	UFUNCTION(BlueprintPure, Category = "Widgets|Text")
		FText GetDisplayText() const;

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetDisplayText(const FText& NewDisplayText);

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
		void SetText(FText& Text, const FText& NewValue);
};
