// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Communicate/CommunicateCallOut.h"
#include "UI/Communicate/CommunicateTalk.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "System/NPCInteractionManager.h"
#include "System/TradeManager.h"
#include "System/UIManager.h"
#include "System/ObserverManager.h"
#include "GameFramework/PlayerController.h"
#include "GameBase/ERGameInstance.h"
#include "Characters/GameCharacter_Interface.h"
#include "Input/InputActionType.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

void UCommunicateCallOut::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)
	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_trade_mgr = instance->GetSubsystem<UTradeManager>();
	CHECK_INVALID_SMART_PTR(m_trade_mgr)
	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)

	m_delay_action.CallbackTarget = this;
	m_delay_action.Linkage = 0;
	m_delay_action.ExecutionFunction = FName("OpenInteractPopUp");

	TalkBox->InitCallOut(this);
}

FReply UCommunicateCallOut::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (EKeys::LeftMouseButton == InMouseEvent.GetEffectingButton() && false == TalkBox->IsPlayAnimation())
	{
		if (m_is_threatened && !m_data_treatened.DialogueText.IsEmpty())
		{
			TArray<FName> names = m_data_treatened.EventFunctions;
			for (const FName& name : names)
			{
				if (name.IsNone()) continue;

				UFunction* func = FindFunction(name);
				if (func) { ProcessEvent(func, nullptr); }
			}
		}
		else { UpdateCommunicate(); }
	}

	return result;
}

void UCommunicateCallOut::StartCommunicate(const TObjectPtr<APlayerController>& Controller)
{
	CHECK_INVALID_PTR(Controller)
	m_controller = Controller;

	IGameCharacter_Interface* character_interface = Cast<IGameCharacter_Interface>(m_controller->AcknowledgedPawn);
	CHECK_INVALID_PTR(character_interface)

	if (character_interface->IsShowingWeapon())
	{
		m_is_threatened = true;
		m_data_cur = m_interact_mgr->GetLastDialouge();
		TalkBox->StartDialouge(FText::FromString(m_data_treatened.DialogueText));
	}
	else
	{
		m_is_threatened = false;
		if (m_interact_mgr->IsFirstInteract())
		{
			m_interact_mgr->DoFirstInteract();
			FCommunicateScriptData* data_cur = m_dialouge->FindRow<FCommunicateScriptData>(FName("Greeting_1"), FName("Greeting_1").ToString());
			CHECK_INVALID_PTR(data_cur)

			m_data_cur = *data_cur;
			TalkBox->StartDialouge(FText::FromString(m_data_cur.DialogueText));
		}
		else
		{
			m_data_cur = m_interact_mgr->GetLastDialouge();
			TalkBox->StartDialouge(FText::FromString(m_data_cur.DialogueText));
		}
	}

	AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(m_controller.Get(), this);
}

void UCommunicateCallOut::StartOptionCommunicate(const FString& Key)
{
	CHECK_INVALID_SMART_PTR(m_controller)
	IGameCharacter_Interface* character_interface = Cast<IGameCharacter_Interface>(m_controller->AcknowledgedPawn);

	if (character_interface->IsShowingWeapon())
	{
		m_is_threatened = true;
		TalkBox->StartDialouge(FText::FromString(m_data_treatened.DialogueText));
	}
	else
	{
		FCommunicateScriptData* data_cur = m_dialouge->FindRow<FCommunicateScriptData>(FName(Key), Key);
		CHECK_INVALID_PTR(data_cur)

		m_data_cur = *data_cur;
		TalkBox->StartDialouge(FText::FromString(m_data_cur.DialogueText));
	}

	AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(m_controller.Get(), this);
}

void UCommunicateCallOut::InitThreatenedDialogue()
{
	m_dialouge = m_interact_mgr->GetDialougeTable();
	CHECK_INVALID_PTR(m_dialouge)

	FCommunicateScriptData* dialouge = m_dialouge->FindRow<FCommunicateScriptData>(FName("Negative"), FName("Negative").ToString());
	CHECK_INVALID_PTR(dialouge)

	m_data_treatened = *dialouge;
}

void UCommunicateCallOut::UpdateCommunicate()
{
	if (m_data_cur.IsTriggerEvent)
	{
		TArray<FName> names = m_data_cur.EventFunctions;
		for (const FName name : names)
		{
			if (name.IsNone()) continue;

			UFunction* func = FindFunction(name);
			CHECK_INVALID_PTR(func)
			ProcessEvent(func, nullptr);

			if (!m_data_cur.Options.IsEmpty())
			{
				FCommunicateScriptData* dialouge = m_dialouge->FindRow<FCommunicateScriptData>(FName(m_data_cur.Options[0]), FName(m_data_cur.Options[0]).ToString());
				CHECK_INVALID_PTR(dialouge)
				m_data_cur = *dialouge;
			}
		}
	}
	else
	{
		FCommunicateScriptData* dialouge = m_dialouge->FindRow<FCommunicateScriptData>(FName(m_data_cur.Options[0]), FName(m_data_cur.Options[0]).ToString());
		CHECK_INVALID_PTR(dialouge)
		m_data_cur = *dialouge;

		TalkBox->UpdateDialouge(FText::FromString(m_data_cur.DialogueText));
	}
}

void UCommunicateCallOut::EndCommunicate()
{
	TalkBox->HideDialouge();
	m_data_treatened = FCommunicateScriptData();
	m_interact_mgr->SetCurrentDialouge(m_data_cur);

	UKismetSystemLibrary::Delay(GetWorld(), 1, m_delay_action);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_controller.Get());
}

void UCommunicateCallOut::EndOptionCommunicate()
{
	TalkBox->HideDialouge();
	m_data_treatened = FCommunicateScriptData();

	FCommunicateScriptData* dialouge = m_dialouge->FindRow<FCommunicateScriptData>(FName(m_data_cur.Options[0]), FName(m_data_cur.Options[0]).ToString());
	CHECK_INVALID_PTR(dialouge)

	m_interact_mgr->SetCurrentDialouge(*dialouge);

	UKismetSystemLibrary::Delay(GetWorld(), 1, m_delay_action);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_controller.Get());
}

void UCommunicateCallOut::SelectOption()
{
	TalkBox->HideDialouge();
	m_data_treatened = FCommunicateScriptData();
	m_interact_mgr->SetCurrentDialouge(m_data_cur);

	if (m_data_cur.ButtonType.IsEmpty() || m_data_cur.ButtonType.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Option Button Type."))
		return;
	}

	m_interact_mgr->OpenSelect(m_data_cur.ButtonType[0], m_data_cur.ButtonType[1]);
}

void UCommunicateCallOut::OpenTrade()
{
	CHECK_INVALID_SMART_PTR(m_controller)

	TalkBox->HideDialouge();
	m_data_treatened = FCommunicateScriptData();
	m_trade_mgr->OpenTrade(m_controller.Get());

	FCommunicateScriptData* data_cur = m_dialouge->FindRow<FCommunicateScriptData>(FName(m_data_cur.Options[0]), m_data_cur.Options[0]);
	CHECK_INVALID_PTR(data_cur)
	m_interact_mgr->SetCurrentDialouge(*data_cur);
}

void UCommunicateCallOut::OpenInteractPopUp()
{
	CHECK_INVALID_SMART_PTR(m_controller)
	m_interact_mgr->OpenInteractPopUp(m_controller.Get(), EInteractPopupType::EIPT_ToNPC);
}

void UCommunicateCallOut::OpenSwapTutorial()
{
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->CloseAllTutorial();

	FTimerHandle timer;
	TDelegate swap_delegate = FTimerDelegate::CreateLambda([&]()->void {
		m_ui_mgr->OpenTutorialCallout(FName("Swap"), EInputActionType::EIAT_Swap, true);
		});
	GetWorld()->GetTimerManager().SetTimer(timer, swap_delegate, 1, false);
}

void UCommunicateCallOut::SaveGameData()
{
	CHECK_INVALID_SMART_PTR(m_controller)

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)

	UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_PTR(observer_mgr)

	instance->ResetSaveData();
	observer_mgr->TriggerEvent(EObserverEventType::EOET_SaveData);
}
