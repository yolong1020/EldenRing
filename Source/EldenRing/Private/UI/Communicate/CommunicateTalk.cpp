// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Communicate/CommunicateTalk.h"
#include "UI/Communicate/CommunicateCallOut.h"
#include "Animation/WidgetAnimation.h"
#include "Delegates/DelegateCombinations.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "System/NPCInteractionManager.h"
#include "Macro/DebugMacros.h"

void UCommunicateTalk::InitCallOut(UCommunicateCallOut* const CallOut)
{
	m_callout = CallOut;
}

void UCommunicateTalk::StartDialouge(const FText& Dialouge)
{
	m_text = Dialouge;
	m_is_playing = true;

	PlayAnimation(ShowCommunicate);
}

void UCommunicateTalk::UpdateDialouge(const FText& Dialouge)
{
	m_text = Dialouge;
	m_is_playing = true;

	PlayAnimation(HideDialogue);
}

void UCommunicateTalk::HideDialouge()
{
	m_is_playing = true;
	PlayAnimation(HideCommunicate);
}

void UCommunicateTalk::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FWidgetAnimationDynamicEvent end_delegate;
	end_delegate.BindDynamic(this, &UCommunicateTalk::EndDialouge);
	BindToAnimationFinished(HideCommunicate, end_delegate);

	FWidgetAnimationDynamicEvent show_delegate;
	show_delegate.BindDynamic(this, &UCommunicateTalk::ShowTalkScript);
	BindToAnimationFinished(ShowCommunicate, show_delegate);

	FWidgetAnimationDynamicEvent hide_delegate;
	hide_delegate.BindDynamic(this, &UCommunicateTalk::ShowTalkScript);
	BindToAnimationFinished(HideDialogue, hide_delegate);

	FWidgetAnimationDynamicEvent finish_delegate;
	finish_delegate.BindDynamic(this, &UCommunicateTalk::FinishAnimation);
	BindToAnimationFinished(ShowDialogue, finish_delegate);

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)
	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
}

void UCommunicateTalk::EndDialouge()
{
	m_is_playing = false;
	m_interact_mgr->SetEnableFramingCamera(false, 1.0f);
	m_callout->RemoveFromParent();
}

void UCommunicateTalk::ShowTalkScript()
{
	TalkScript->SetText(m_text);
	StopAllAnimations();
	PlayAnimation(ShowDialogue);
}
