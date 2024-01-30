// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/Title.h"
#include "UI/Button/MenuButton.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Animation/UMGSequencePlayer.h"
#include "Controller/TitleController.h"
#include "Input/Reply.h"
#include "Macro/DebugMacros.h"

void UTitle::ShowTitle()
{
	CHECK_INVALID_PTR(ShowStartTitle)
	m_sequence = PlayAnimation(ShowStartTitle);

	SetFocus();
}

const bool UTitle::IsPlayingAnimation() const
{
	return !m_sequence.IsValid() ? false : EMovieScenePlayerStatus::Playing == m_sequence->GetPlaybackStatus();
}

void UTitle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FWidgetAnimationDynamicEvent callback;
	callback.BindUFunction(this, FName("InitButtons"));
	BindToAnimationFinished(ShowStartTitle, callback);

	FActivateMenuButtonFunc activated_callback;
	activated_callback.BindUFunction(this, FName("SetActivation"));
	FCheckActivateMenuButtonFunc check_callback;
	check_callback.BindUFunction(this, FName("IsActivated"));
	FCHeckAnimMenuButtonFunc anim_callback;
	anim_callback.BindUFunction(this, FName("IsPlayingAnimation"));

	NewGame->InitCallback(activated_callback, check_callback, anim_callback);
	LoadGame->InitCallback(activated_callback, check_callback, anim_callback);
	Exit->InitCallback(activated_callback, check_callback, anim_callback);
}