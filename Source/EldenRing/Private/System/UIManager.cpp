// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UIManager.h"
#include "UI/Common/BlackOut.h"
#include "UI/PopUp/PopUp.h"
#include "UI/Notify/SubNotify.h"
#include "UI/Tutorial/TutorialPersistent.h"
#include "UI/Tutorial/TutorialCallout.h"
#include "HUD/LockOn/LockOn.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameBase/ERGameInstance.h"
#include "System/ObserverManager.h"
#include "Macro/DebugMacros.h"

const bool UUIManager::IsPlayingBalckout() const
{
	return !m_black_out ? false : m_black_out->IsPlayingAnimation();
}

const bool UUIManager::IsBlackOutInViewport() const
{
	return m_black_out->IsInViewport();
}

void UUIManager::ShowBlackOut(const FAfterShowBlackOutFunc& Callback)
{
	if (!m_black_out) { CreateBlackOut(); }

	if (m_black_out->IsPlayingAnimation()) return;

	m_black_out->ShowBlackOut(Callback);
}

void UUIManager::ShowBlackOutNoAnimation()
{
	if (!m_black_out) { CreateBlackOut(); }

	if (m_black_out->IsPlayingAnimation()) return;

	m_black_out->ShowBlackOutNoAnimation();
}

void UUIManager::HideBlackOut(const FAfterHideBlackOutFunc& Callback)
{
	if (!m_black_out) { CreateBlackOut(); }

	if (m_black_out->IsPlayingAnimation()) return;

	m_black_out->HideBlackOut(Callback);
}

void UUIManager::RegistUI(TWeakObjectPtr<UUserWidget> Widget, const int32& ZOrder)
{
	m_container.Add(Widget);
	Widget->AddToViewport(ZOrder);
}

void UUIManager::UnregistUI()
{
	if (m_container.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty UI"))
	}
	else
	{
		TWeakObjectPtr<UUserWidget> widget = m_container.Last();
		if (widget.Get())
		{
			widget->RemoveFromParent();
			m_container.Pop();
		}
	}
}

void UUIManager::OpenExitPopUp(const TWeakObjectPtr<APlayerController>& Controller)
{
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller.Get());
	Controller->bShowMouseCursor = true;

	FPopupActiveBtn active_callback;
	active_callback.BindLambda([&]()->void {
			UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
			CHECK_INVALID_PTR(instance)
			UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
			CHECK_INVALID_PTR(observer_mgr)

			instance->SavePlayer(Controller->AcknowledgedPawn);
			instance->ResetSaveData();
			observer_mgr->TriggerEvent(EObserverEventType::EOET_SaveData);

			OpenSubNotify(FText::FromString(TEXT("10초 뒤 게임이 종료됩니다.")));

			FLatentActionInfo callback;
			callback.CallbackTarget = this;
			callback.ExecutionFunction = FName("ExitPopUpAction");
			callback.Linkage = 0;
			callback.UUID = FGuid::NewGuid().A;
			UKismetSystemLibrary::Delay(this, 10.f, callback);

			UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller.Get());
			Controller->bShowMouseCursor = false;
		});

	FPopupCancelBtn cancel_callback;
	cancel_callback.BindLambda([&]()->void {
			UnregistUI();

			UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller.Get());
			Controller->bShowMouseCursor = false;
		});

	OpenTwoBtnPopUp(EPopupType::EPT_ExitGame, active_callback, cancel_callback);
}

void UUIManager::OpenOneBtnPopUp(const EPopupType& Type, const FPopupConfirmlBtn& Confirm)
{
	if (!m_popup) { CreatePopUp(); }

	m_popup->UpdateOneButtonLayout(Type, Confirm);
	RegistUI(m_popup);
}

void UUIManager::OpenTwoBtnPopUp(const EPopupType& Type, const FPopupActiveBtn& Active, const FPopupCancelBtn& Cancel)
{
	if (!m_popup) { CreatePopUp(); }

	m_popup->UpdateTwoButtonForExitGame(Type, Active, Cancel);
	RegistUI(m_popup);
}

void UUIManager::OpenSubNotify(const FText& InText)
{
	if (!m_sub_notify)
	{
		UClass* notify_class = LoadClass<USubNotify>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Notify/WBP_SubNotify.WBP_SubNotify_C'"));
		CHECK_INVALID_PTR(notify_class)
		m_sub_notify = CreateWidget<USubNotify>(GetWorld(), notify_class);
		CHECK_INVALID_PTR(m_sub_notify)
	}

	if (false == m_sub_notify->IsInViewport()) { m_sub_notify->ShowSubNotify(InText); }
}

void UUIManager::OpenTutorialGroupCall(const TArray<FTutorialPersistentData>& Slides)
{
	if (!m_tutorial_group) { CreateTutorialGroupCall(); }

	m_tutorial_group->Show(Slides);
}

void UUIManager::CloseTutorialGroupCall()
{
	if (!m_tutorial_group || false == m_tutorial_group->IsActivated()) return;
	
	m_tutorial_group->Hide();
}

void UUIManager::OpenTutorialCallout(const FName& Name, const EInputActionType& Type, const bool IsLargeKey, const float& Duration)
{
	if (!m_callout)					{ CreateTutorialCallout(); }
	if (!m_callout->IsInViewport()) { m_callout->AddToViewport(); }

	m_callout->OriginalSetUp(Name, Type, IsLargeKey, Duration);
}

void UUIManager::CloseTutorialCallout()
{
	if (!m_callout || false == m_callout->IsActivated()) return;

	m_callout->HideInteraction();
}

const bool UUIManager::IsActiveCallout(const FName& Name)
{
	if (!m_callout) return false;

	return m_callout->IsActivated(Name);
}

void UUIManager::CloseAllTutorial()
{
	if (m_tutorial_group && m_tutorial_group->IsActivated())
	{
		m_tutorial_group->Hide();
	}

	if (m_callout && m_callout->IsActivated())
	{
		m_callout->HideInteraction();
	}
}

void UUIManager::ShowLockOn(const TWeakObjectPtr<APlayerController> Controller, const TWeakObjectPtr<AGameCharacter> Target)
{
	if (!m_lockon)
	{
		UClass* widget_type = LoadClass<ULockOn>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/HUD/LockOn/WBP_LockOn.WBP_LockOn_C'"));
		CHECK_INVALID_PTR(widget_type)
		m_lockon = CreateWidget<ULockOn>(GetWorld(), widget_type);
		CHECK_INVALID_PTR(m_lockon)
	}

	m_lockon->SetPosition(Controller, Target);
}

void UUIManager::HideLockOn()
{
	if (m_lockon && m_lockon->IsInViewport()) { m_lockon->RemoveFromParent(); }
}

void UUIManager::CreateTutorialCallout()
{
	UClass* widget_type = LoadClass<UTutorialCallout>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Tutorial/WBP_TutorialCallout.WBP_TutorialCallout_C'"));
	CHECK_INVALID_PTR(widget_type)
	m_callout = CreateWidget<UTutorialCallout>(GetWorld(), widget_type);
	CHECK_INVALID_PTR(m_callout)
}

void UUIManager::CreateBlackOut()
{
	UClass* widget_type = LoadClass<UBlackOut>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Common/BlackOut/WBP_BlackOut.WBP_BlackOut_C'"));
	CHECK_INVALID_PTR(widget_type)
	m_black_out = CreateWidget<UBlackOut>(GetWorld(), widget_type);
	CHECK_INVALID_PTR(m_black_out)
}

void UUIManager::CreatePopUp()
{
	UClass* widget_type = LoadClass<UPopUp>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Popup/Common/WBP_Popup.WBP_Popup_C'"));
	CHECK_INVALID_PTR(widget_type)
	m_popup = CreateWidget<UPopUp>(GetWorld(), widget_type);
	CHECK_INVALID_PTR(m_popup)
}

void UUIManager::CreateTutorialGroupCall()
{
	UClass* widget_class = LoadClass<UTutorialPersistent>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Tutorial/WBP_TutorialPersistent.WBP_TutorialPersistent_C'"));
	CHECK_INVALID_PTR(widget_class)
	m_tutorial_group = CreateWidget<UTutorialPersistent>(GetWorld(), widget_class);
	CHECK_INVALID_PTR(m_tutorial_group)
	m_tutorial_group->AddToViewport();
}

void UUIManager::ExitPopUpAction()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
