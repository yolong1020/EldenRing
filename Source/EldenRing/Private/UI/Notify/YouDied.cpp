// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Notify/YouDied.h"
#include "UI/Common/BlackOut.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "System/ObserverManager.h"
#include "System/NPCInteractionManager.h"
#include "System/SoundscapeManager.h"
#include "Macro/DebugMacros.h"

void UYouDied::StartDirecting(const FAfterPlayerDeathFunc& Callback, const float& DelayTime)
{
	if (!Callback.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("FAfterPlayerDeathFunc is not bounded."))
		return;
	}

	USoundBase* sound = LoadObject<USoundBase>(nullptr, TEXT("MetaSoundSource'/Game/EldenRing/Sounds/UI/Notify/YouDied/sfx_YouDied.sfx_YouDied'"));
	CHECK_INVALID_PTR(sound)
	UGameInstance* instance = GetGameInstance();
	CHECK_INVALID_PTR(instance)
	USoundscapeManager* sound_mgr = instance->GetSubsystem<USoundscapeManager>();
	CHECK_INVALID_PTR(sound_mgr)
	sound_mgr->PlayNofityBGM(sound, true);

	m_directing_callback = Callback;
	m_delay_time = DelayTime;

	PlayAnimation(ShowHideNotify);
	AddToViewport();
}

void UYouDied::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FWidgetAnimationDynamicEvent end_delegate;
	end_delegate.BindDynamic(this, &UYouDied::EndDirecting);
	BindToAnimationFinished(ShowHideNotify, end_delegate);

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)
	m_observe_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_SMART_PTR(m_observe_mgr)

	UClass* widget_type = LoadClass<UBlackOut>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Common/BlackOut/WBP_BlackOut.WBP_BlackOut_C'"));
	CHECK_INVALID_PTR(widget_type)
	m_black_out = CreateWidget<UBlackOut>(GetWorld(), widget_type);
}

void UYouDied::EndDirecting()
{
	RemoveFromParent();

	m_black_out->ShowBlackOut();

	FLatentActionInfo action;
	action.CallbackTarget = this;
	action.Linkage = 0;
	action.ExecutionFunction = FName("FilledBlackOut");
	UKismetSystemLibrary::Delay(GetWorld(), m_delay_time, action);
}

void UYouDied::FilledBlackOut()
{
	m_black_out->HideBlackOut(FAfterHideBlackOutFunc());
	m_directing_callback.ExecuteIfBound();
}
