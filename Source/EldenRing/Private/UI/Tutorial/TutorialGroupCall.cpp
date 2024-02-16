// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Tutorial/TutorialPersistent.h"
#include "UI/Tutorial/TutorialHUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/UIManager.h"
#include "TimerManager.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

const bool UTutorialGroupCall::IsActivated() const
{
	return (UIP_SlideTop->IsActived() || UIP_Slide_Mid->IsActived() || UIP_Slide_Bot->IsActived());
}

void UTutorialGroupCall::Show(const TArray<FTutorialPersistentData>& Slides)
{
	m_slides.Empty(3);

	for (const FTutorialPersistentData& data : Slides)
	{
		switch (data.ShowPosition)
		{
			case ETutorialPosition::ETP_Top:
			{
				UIP_SlideTop->UpdateButton(data.Name, data.Action, data.IsLargeKey);
				UIP_SlideTop->ShowTutorial();
			} break;
			case ETutorialPosition::ETP_Mid:
			{
				UIP_Slide_Mid->UpdateButton(data.Name, data.Action, data.IsLargeKey);
				UIP_Slide_Mid->ShowTutorial();
			} break;
			case ETutorialPosition::ETP_Bot:
			{
				UIP_Slide_Bot->UpdateButton(data.Name, data.Action, data.IsLargeKey);
				UIP_Slide_Bot->ShowTutorial();
			} break;
		}

		m_slides.Add(data);
	}

	float duration = TutorialNotifyDuration;

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	FTimerManager& time_mgr = world->GetTimerManager();
	time_mgr.ClearTimer(m_timer);
	time_mgr.SetTimer(m_timer, this, &UTutorialPersistent::Hide, duration);
}

void UTutorialGroupCall::Hide()
{
	if (m_slides.IsEmpty()) return;

	FTimerManager& time_mgr = GetWorld()->GetTimerManager();
	time_mgr.ClearTimer(m_timer);

	for (const FTutorialPersistentData& data : m_slides)
	{
		switch (data.ShowPosition)
		{
		case ETutorialPosition::ETP_Top:
			UIP_SlideTop->HideTutorial();
			break;
		case ETutorialPosition::ETP_Mid:
			UIP_Slide_Mid->HideTutorial();
			break;
		case ETutorialPosition::ETP_Bot:
			UIP_Slide_Bot->HideTutorial();
			break;
		}
	}

	m_slides.Empty();
}

void UTutorialGroupCall::NativeConstruct()
{
	Super::NativeConstruct();

	UIP_SlideTop->HideUI();
	UIP_Slide_Bot->HideUI();
	UIP_Slide_Mid->HideUI();
}
