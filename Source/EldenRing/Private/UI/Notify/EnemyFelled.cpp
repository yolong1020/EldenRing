// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Notify/EnemyFelled.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "System/ObserverManager.h"
#include "System/NPCInteractionManager.h"
#include "System/SoundscapeManager.h"
#include "Macro/DebugMacros.h"

void UEnemyFelled::StartDirecting()
{
	FLatentActionInfo action;
	action.CallbackTarget = this;
	action.Linkage = 0;
	action.ExecutionFunction = FName("ShowDirecting");
	UKismetSystemLibrary::Delay(GetWorld(), m_delay_time, action);
}

void UEnemyFelled::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FWidgetAnimationDynamicEvent end_delegate;
	end_delegate.BindDynamic(this, &UEnemyFelled::EndDirecting);
	BindToAnimationFinished(ShowHideNotify, end_delegate);

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)
	m_observe_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_PTR(m_observe_mgr)
	// m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	// CHECK_INVALID_PTR(m_interact_mgr)
}

void UEnemyFelled::ShowDirecting()
{
	USoundBase* sound = LoadObject<USoundBase>(nullptr, TEXT("MetaSoundSource'/Game/EldenRing/Sounds/UI/Notify/BossFelled/sfx_BossFelled.sfx_BossFelled'"));
	CHECK_INVALID_PTR(sound)
	UGameInstance* instance = GetGameInstance();
	CHECK_INVALID_PTR(instance)
	USoundscapeManager* sound_mgr = instance->GetSubsystem<USoundscapeManager>();
	CHECK_INVALID_PTR(sound_mgr)
	sound_mgr->PlayNofityBGM(sound, true);

	PlayAnimation(ShowHideNotify);
	AddToViewport();
}

void UEnemyFelled::EndDirecting()
{
	RemoveFromParent();

	m_observe_mgr->TriggerEvent(EObserverEventType::EOET_EnemyFelled);
	// m_interact_mgr->OpenInteractPopUp(EInteractPopupType::EIPT_ToBonefire, false);
}