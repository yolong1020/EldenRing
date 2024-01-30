// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/CommonButton.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "HUD/GameHUD_Actor.h"
#include "Components/Button.h"
#include "Macro/DebugMacros.h"

void UCommonButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonText(TextContext);
}

void UCommonButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UCommonButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UCommonButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	Button->OnClicked.Broadcast();
	return result;
}

void UCommonButton::BindButtonFunc(const FScriptDelegate Func)
{
	CHECK_INVALID_PTR(Button)

	FOnButtonClickedEvent click_event;
	click_event.Add(Func);

	Button->OnClicked = click_event;
}
