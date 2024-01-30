// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Button/BoneFireButton.h"

void UBoneFireButton::BindButtonFunc(const FBoneFireButtonFunc& Func)
{
	m_func.Unbind();
	m_func = Func;
}

void UBoneFireButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBoneFireButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateButtonText(TextContext);
}

FReply UBoneFireButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (EKeys::LeftMouseButton == InMouseEvent.GetEffectingButton() && m_func.IsBound())
	{
		m_func.Execute();
	}

	return result;
}


void UBoneFireButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	StopAllAnimations();
	PlayAnimation(MouseEnter);
}

void UBoneFireButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	StopAllAnimations();
	PlayAnimation(MouseLeave);
}
