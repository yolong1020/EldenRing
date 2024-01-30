// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Tutorial/TutorialCallout.h"
#include "UI/Tutorial/TutorialHUD.h"
#include "Macro/DebugMacros.h"

const bool UTutorialCallout::IsActivated() const
{
	if (!UIP_SlideTop) return false;

	return UIP_SlideTop->IsActived();
}

const bool UTutorialCallout::IsActivated(const FName& Name) const
{
	if (!UIP_SlideTop) return false;

	return Name == UIP_SlideTop->GetText().ToString();
}

void UTutorialCallout::OriginalSetUp(const FName& Name, const EInputActionType& Type, const bool IsLargeKey, const float& Duration)
{
	CHECK_INVALID_PTR(UIP_SlideTop)

	UIP_SlideTop->UpdateButton(Name, Type, IsLargeKey);
	UIP_SlideTop->ShowTutorial(Duration);
}

void UTutorialCallout::HideInteraction()
{
	CHECK_INVALID_PTR(UIP_SlideTop)

	UIP_SlideTop->HideTutorial();
}
