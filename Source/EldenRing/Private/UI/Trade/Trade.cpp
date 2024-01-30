// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trade/Trade.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "System/TradeManager.h"
#include "System/NPCInteractionManager.h"
#include "System/UIManager.h"
#include "GameFramework/Character.h"
#include "Macro/DebugMacros.h"

void UTrade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_callback.CallbackTarget = this;
	m_callback.Linkage = 0;
	m_callback.UUID = FGuid::NewGuid().A;
	m_callback.ExecutionFunction = FName("ExitTrade");

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	m_trade_mgr = instance->GetSubsystem<UTradeManager>();
	CHECK_INVALID_SMART_PTR(m_trade_mgr)
	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
}

FReply UTrade::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	CHECK_INVALID_SMART_PTR_RetVal(m_ui_mgr)
	if (EKeys::Escape == UKismetInputLibrary::GetKey(InKeyEvent) && m_ui_mgr->IsTopUI(this))
	{
		UKismetSystemLibrary::Delay(this, 0.1f, m_callback);
	}

	return result;
}

void UTrade::ExitTrade()
{
	CHECK_INVALID_SMART_PTR(m_trade_mgr)
	m_trade_mgr->HideItemInfo();

	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->UnregistUI();

	CHECK_INVALID_SMART_PTR(m_controller)
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_controller.Get());
	m_controller->bShowMouseCursor = false;

	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_interact_mgr->OpenInteractPopUp(m_controller.Get(), EInteractPopupType::EIPT_ToNPC);
}
