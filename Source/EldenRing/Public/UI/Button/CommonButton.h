// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonType.h"
#include "CommonButton.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ELDENRING_API UCommonButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindButtonFunc(const FScriptDelegate Func);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);
		
protected:
	virtual void NativePreConstruct() override final;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override final;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;

private:
	UPROPERTY(BlueprintReadWrite, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* TextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FText TextContext;
};
