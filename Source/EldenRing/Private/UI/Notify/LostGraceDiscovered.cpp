// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Notify/LostGraceDiscovered.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "System/ObserverManager.h"
#include "System/NPCInteractionManager.h"
#include "System/SoundscapeManager.h"
#include "Macro/DebugMacros.h"

void ULostGraceDiscovered::StartDirecting(const TObjectPtr<APlayerController>& Controller)
{
	CHECK_INVALID_PTR(Controller)
	m_controller = Controller;

	FLatentActionInfo action;
	action.CallbackTarget = this;
	action.Linkage = 0;
	action.ExecutionFunction = FName("ShowDirecting");
	UKismetSystemLibrary::Delay(GetWorld(), m_delay_time, action);

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(m_controller.Get(), this);
}

void ULostGraceDiscovered::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FWidgetAnimationDynamicEvent end_delegate;
	end_delegate.BindDynamic(this, &ULostGraceDiscovered::EndDirecting);
	BindToAnimationFinished(ShowHideNotify, end_delegate);

	UGameInstance* instance =UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)
	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
}

void ULostGraceDiscovered::EndDirecting()
{
	CHECK_INVALID_SMART_PTR(m_controller)

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_controller.Get());
	RemoveFromParent();

	m_interact_mgr->OpenInteractPopUp(m_controller.Get(), EInteractPopupType::EIPT_ToBonefire, false);
}

void ULostGraceDiscovered::ShowDirecting()
{
	USoundBase* sound = LoadObject<USoundBase>(nullptr, TEXT("MetaSoundSource'/Game/EldenRing/Sounds/UI/Notify/Bonefire/sfx_BonefireDiscovered.sfx_BonefireDiscovered'"));
	CHECK_INVALID_PTR(sound)
	UGameInstance* instance = GetGameInstance();
	CHECK_INVALID_PTR(instance)
	USoundscapeManager* sound_mgr = instance->GetSubsystem<USoundscapeManager>();
	CHECK_INVALID_PTR(sound_mgr)
	sound_mgr->PlayNofityBGM(sound, false);

	PlayAnimation(ShowHideNotify);
	AddToViewport();
}
