// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PopUp/SelectionPopUp.h"
#include "UI/Button/ButtonType.h"
#include "UI/Button/SelectionButton.h"
#include "Kismet/GameplayStatics.h"
#include "System/TradeManager.h"
#include "System/NPCInteractionManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Macro/DebugMacros.h"

void USelectionPopUp::ShowPopUp(const TObjectPtr<APlayerController>& Controller)
{
	CHECK_INVALID_PTR(Controller)
	m_controller = Controller;

	AddToViewport();

	PlayAnimation(StartShowSelection);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(m_controller, this);
}

void USelectionPopUp::SetOptionButtons(const uint8& FirstType, const uint8& SecondType)
{
	FindButtonFunction(FirstOption, FirstType);
	FindButtonFunction(SecondOption, SecondType);
}

void USelectionPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)

	m_trade_mgr = instance->GetSubsystem<UTradeManager>();
	CHECK_INVALID_PTR(m_trade_mgr)

	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
}

FReply USelectionPopUp::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (IsPlayingAnimation()) return result;

	if		(EKeys::E == InKeyEvent.GetKey())		{ OnConfirm(); }
	else if (EKeys::Escape == InKeyEvent.GetKey())	{ OnEscape(); }

	return result;
}

void USelectionPopUp::FindButtonFunction(USelectionButton* const button, const uint8& Type)
{
	FSelectButtonFunc func;
	switch ((ESelectPopupButtonType)Type)
	{
		case ESelectPopupButtonType::ESPBT_ToCommunicate:
			func.BindLambda([&]()->void { 
					UnbindAllFromAnimationFinished(StartHideSelection);

					FWidgetAnimationDynamicEvent hide_callback;
					hide_callback.BindDynamic(this, &USelectionPopUp::CloseAndCommunicate);
					BindToAnimationFinished(StartHideSelection, hide_callback);
					PlayAnimation(StartHideSelection);
				});
			break;
		case ESelectPopupButtonType::ESPBT_ToTrade:
			func.BindLambda([&]()->void {
					UnbindAllFromAnimationFinished(StartHideSelection);

					FWidgetAnimationDynamicEvent hide_callback;
					hide_callback.BindDynamic(this, &USelectionPopUp::CloseAndTrade);
					BindToAnimationFinished(StartHideSelection, hide_callback);
					PlayAnimation(StartHideSelection);
				});
			break;
		case ESelectPopupButtonType::ESPBT_Max:
		default: return;
	}

	button->BindButtonFunc(func);
}

void USelectionPopUp::ClosePopUp()
{
	CHECK_INVALID_PTR(m_controller)

	RemoveFromParent();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_controller);
	m_interact_mgr->OpenInteractPopUp(m_controller, EInteractPopupType::EIPT_ToNPC);
}

void USelectionPopUp::CloseAndCommunicate()
{
	m_interact_mgr->SetEnableFramingCamera(true, 1.0f);
	m_interact_mgr->OpenCommunicateByOption();

	RemoveFromParent();
}

void USelectionPopUp::CloseAndTrade()
{
	UDataTable* table = m_interact_mgr->GetDialougeTable();
	FCommunicateScriptData* dialouge = table->FindRow<FCommunicateScriptData>(FName("Shop"), FName("Shop").ToString());
	CHECK_INVALID_PTR(dialouge)

	m_interact_mgr->SetCurrentDialouge(*dialouge);
	m_interact_mgr->OpenCommunicate();

	RemoveFromParent();
}

void USelectionPopUp::OnEscape()
{
	UnbindAllFromAnimationFinished(StartHideSelection);

	FWidgetAnimationDynamicEvent hide_callback;
	hide_callback.BindDynamic(this, &USelectionPopUp::ClosePopUp);
	BindToAnimationFinished(StartHideSelection, hide_callback);

	PlayAnimation(StartHideSelection);
}

void USelectionPopUp::OnConfirm()
{

}


