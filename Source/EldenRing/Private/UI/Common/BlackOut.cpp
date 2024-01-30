// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/BlackOut.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/Border.h"
#include "Macro/DebugMacros.h"

void UBlackOut::ShowBlackOutNoAnimation()
{
	CHECK_INVALID_PTR(BlackBorder)
	BlackBorder->SetRenderOpacity(1.f);

	if (!IsInViewport()) { AddToViewport(100); }
}

void UBlackOut::ShowBlackOut()
{
	m_sequence = PlayAnimation(ShowStartBlackOut);
	if (!IsInViewport()) { AddToViewport(100); }
}

void UBlackOut::ShowBlackOut(const FAfterShowBlackOutFunc& Callback)
{
	m_sequence = PlayAnimation(ShowStartBlackOut);
	if (!IsInViewport()) { AddToViewport(100); }

	m_show_callback = Callback;
}

void UBlackOut::HideBlackOut(const FAfterHideBlackOutFunc& Callback)
{
	UE_LOG(LogTemp, Warning, TEXT("Hide BlackOut"))

	StopAnimation(ShowStartBlackOut);
	m_sequence = PlayAnimation(HideStartBlackOut);

	if (!IsInViewport()) { AddToViewport(100); }

	m_hide_callback = Callback;
}

const bool UBlackOut::IsPlayingAnimation() const
{
	return !m_sequence.IsValid() ? false : EMovieScenePlayerStatus::Playing == m_sequence->GetPlaybackStatus();
}

void UBlackOut::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FWidgetAnimationDynamicEvent show_callback;
	show_callback.BindDynamic(this, &UBlackOut::EndShowBlackOut);
	BindToAnimationFinished(ShowStartBlackOut, show_callback);

	FWidgetAnimationDynamicEvent hide_callback;
	hide_callback.BindDynamic(this, &UBlackOut::EndHideBlackOut);
	BindToAnimationFinished(HideStartBlackOut, hide_callback);
}

void UBlackOut::EndShowBlackOut()
{
	m_show_callback.ExecuteIfBound();
}

void UBlackOut::EndHideBlackOut()
{
	m_hide_callback.ExecuteIfBound();
	RemoveFromParent();
}
