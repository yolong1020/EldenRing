// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Notify/SubNotify.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/AudioComponent.h"
#include "Macro/DebugMacros.h"

void USubNotify::ShowSubNotify(const FText& InText)
{
	AddToViewport();
	Message->SetText(InText);

	UAudioComponent* notify_component = UGameplayStatics::CreateSound2D(GetWorld(), NotifySound);
	CHECK_INVALID_PTR(notify_component)
	notify_component->bAutoDestroy = true;
	notify_component->bAllowSpatialization = false;
	notify_component->Play();

	PlayAnimation(StartShowNotify);
}

void USubNotify::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_delay_callback.Linkage = 0;
	m_delay_callback.CallbackTarget = this;
	m_delay_callback.ExecutionFunction = FName("HideSubNotify");

	FWidgetAnimationDynamicEvent show_callback;
	show_callback.BindDynamic(this, &USubNotify::DelayNextProcess);
	BindToAnimationFinished(StartShowNotify, show_callback);

	FWidgetAnimationDynamicEvent close_callback;
	close_callback.BindDynamic(this, &USubNotify::CloseSubNotify);
	BindToAnimationFinished(StartHideNotify, close_callback);
}

void USubNotify::HideSubNotify()
{
	PlayAnimation(StartHideNotify);
}

void USubNotify::DelayNextProcess()
{
	UKismetSystemLibrary::Delay(GetWorld(), 3, m_delay_callback);
}

void USubNotify::CloseSubNotify()
{
	RemoveFromParent();
}
