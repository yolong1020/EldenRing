// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/SelectionButton.h"
#include "Components/TextBlock.h"
#include "Macro/DebugMacros.h"

void USelectionButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonText(TextContext);
}

void USelectionButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	StopAllAnimations();
	PlayAnimation(MouseEnter);
}

void USelectionButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	StopAllAnimations();
	PlayAnimation(MouseLeave);
}

FReply USelectionButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (m_function.IsBound()) { m_function.Execute(); }
	return result;
}

void USelectionButton::BindButtonFunc(const FSelectButtonFunc& Func)
{
	m_function.Unbind();
	m_function = Func;
}
