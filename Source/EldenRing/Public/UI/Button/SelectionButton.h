// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "SelectionButton.generated.h"

UCLASS()
class ELDENRING_API USelectionButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindButtonFunc(const FSelectButtonFunc& Func);

protected:
	virtual void NativePreConstruct() override final;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override final;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);

private:
	UPROPERTY(BlueprintReadWrite, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* BtnName;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UOverlay* Button;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UImage* BtnHover;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* MouseEnter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"), Transient)
	UWidgetAnimation* MouseLeave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FText TextContext;

	FSelectButtonFunc m_function;
};
