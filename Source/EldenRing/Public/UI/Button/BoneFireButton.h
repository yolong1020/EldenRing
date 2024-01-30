// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "BoneFireButton.generated.h"


UCLASS()
class ELDENRING_API UBoneFireButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindButtonFunc(const FBoneFireButtonFunc& Func);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);
	
protected:
	virtual void NativeOnInitialized() override final;
	virtual void NativePreConstruct() override final;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override final;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess))
	class UTextBlock* ButtonName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FText TextContext;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), meta = (AllowPrivateAccess), Transient)
	UWidgetAnimation* MouseEnter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), meta = (AllowPrivateAccess), Transient)
	UWidgetAnimation* MouseLeave;

	FBoneFireButtonFunc m_func;
};
