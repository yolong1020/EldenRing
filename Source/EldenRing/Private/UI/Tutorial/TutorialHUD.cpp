// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Tutorial/TutorialHUD.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Input/IconDataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Macro/DebugMacros.h"

const FText UTutorialHUD::GetText() const
{
	return TXT->GetText();
}

void UTutorialHUD::UpdateButton(const FName& Name, const EInputActionType& Type, const bool IsLargeKey)
{
	CHECK_INVALID_PTR(IconData)
	TArray<FIconData>* arr = IconData->GetIconDataArr();
	CHECK_INVALID_PTR(arr)

	int32 idx = UKismetMathLibrary::Conv_ByteToInt((uint8)Type);
	if (arr->IsValidIndex(idx)) {
		TXT->SetText(FText::FromName(Name));

		TObjectPtr<UImage> icon = IsLargeKey ? Icon_Large : Icon;
		icon->SetBrushFromMaterial((*arr)[idx].MaterialInstance);
		Icon_Large->SetVisibility(IsLargeKey ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		Icon->SetVisibility(IsLargeKey ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

		PressBar->SetPercent(0);
	}
}

void UTutorialHUD::ShowTutorial(const float& Duration)
{
	m_duration = Duration;
	PlayAnimation(ShowInteraction);

	m_is_actived = true;
}

void UTutorialHUD::HideTutorial()
{
	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	FTimerManager& time_mgr = world->GetTimerManager();
	time_mgr.ClearTimer(m_timer);

	if (IsAnimationPlaying(ShowInteraction)) { StopAnimation(ShowInteraction); }

	PlayAnimation(HideInteraction);
}

void UTutorialHUD::HideUI()
{
	m_is_actived = false;
	Head->SetRenderOpacity(0);
}

void UTutorialHUD::SwapVisibility(const bool& On)
{
	DiamondBox->SetVisibility(On ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UTutorialHUD::NativePreConstruct()
{
	Super::NativePreConstruct();
	DiamondBox->SetVisibility(m_has_diamond ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UTutorialHUD::NativeConstruct()
{
	Super::NativeConstruct();
	HideUI();
}

void UTutorialHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FWidgetAnimationDynamicEvent show_delegate;
	show_delegate.BindDynamic(this, &UTutorialHUD::OpenedTutorial);
	BindToAnimationFinished(ShowInteraction, show_delegate);

	FWidgetAnimationDynamicEvent hide_delegate;
	hide_delegate.BindDynamic(this, &UTutorialHUD::ClosedTutorial);
	BindToAnimationFinished(HideInteraction, hide_delegate);
}

void UTutorialHUD::OpenedTutorial()
{
	if (m_duration == 0) return;

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	FTimerManager& time_mgr = world->GetTimerManager();
	time_mgr.ClearTimer(m_timer);
	time_mgr.SetTimer(m_timer, this, &UTutorialHUD::HideTutorial, m_duration);
}

void UTutorialHUD::ClosedTutorial()
{
	m_is_actived = false;
}
