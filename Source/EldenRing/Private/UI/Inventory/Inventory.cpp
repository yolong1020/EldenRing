// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/Inventory.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInputLibrary.h"
#include "GameFramework/Character.h"
#include "System/StorageManager.h"
#include "System/UIManager.h"
#include "Macro/DebugMacros.h"

void UInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_callback.CallbackTarget = this;
	m_callback.Linkage = 0;
	m_callback.UUID = FGuid::NewGuid().A;
	m_callback.ExecutionFunction = FName("ExitInventory");

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
}

FReply UInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	CHECK_INVALID_SMART_PTR_RetVal(m_ui_mgr)
	if ((EKeys::Escape == UKismetInputLibrary::GetKey(InKeyEvent) || 
		 EKeys::I == UKismetInputLibrary::GetKey(InKeyEvent)) &&
		m_ui_mgr->IsTopUI(this))
	{
		UKismetSystemLibrary::Delay(this, 0.1f, m_callback);
	}

	return result;
}

void UInventory::ExitInventory()
{
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_storage_mgr->HideItemInfo();

	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	m_ui_mgr->UnregistUI();

	CHECK_INVALID_SMART_PTR(m_controller)
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_controller.Get());
	m_controller->bShowMouseCursor = false;
}
