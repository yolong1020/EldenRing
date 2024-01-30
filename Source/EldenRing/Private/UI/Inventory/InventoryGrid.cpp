// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryGrid.h"
#include "UI/Inventory/InventorySlotGrid.h"
#include "UI/Inventory/InventoryInteractionGrid.h"
#include "UI/Inventory/InventoryItem.h"
#include "UI/Structure/InventoryLine.h"
#include "UI/Structure/InventoryTile.h"
#include "UI/PopUp/ModalPopUp.h"
#include "HUD/HUD_Function.h"
#include "Items/ItemObject.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "GameBase/ERGameState.h"
#include "Characters/Players/C0000.h"
#include "Slate/SlateBrushAsset.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "System/StorageManager.h"
#include "System/UIManager.h"
#include "Macro/DebugMacros.h"

void UInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CHECK_INVALID_PTR(ButtonGoldInteraction)
	ButtonGoldInteraction->OnClicked.AddDynamic(this, &UInventoryGrid::OnClickedGoldInteraction);

	UGameInstance* instance = GetGameInstance();
	CHECK_INVALID_PTR(instance)

	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
}

void UInventoryGrid::InitGridWidget()
{
	UWidget* widget_slot_grid = GetWidgetFromName(FName("WBP_Inventory_SlotGrid"));
	m_widget_slot_grid = Cast<UInventorySlotGrid>(widget_slot_grid);
	CHECK_INVALID_PTR(m_widget_slot_grid)
	m_widget_slot_grid->InitGridSlotWidget();
	UWidget* widget_Interaction_grid = GetWidgetFromName(FName("WBP_Inventory_InteractionGrid"));
	m_widget_Interaction_grid = Cast<UInventoryInteractionGrid>(widget_Interaction_grid);
	CHECK_INVALID_PTR(m_widget_Interaction_grid)
	m_widget_Interaction_grid->InitInteractionGridWidget(m_margin);

	UGameInstance* instance = UGameplayStatics::GetGameInstance(GetWorld());
	CHECK_INVALID_PTR(instance)
	UStorageManager* storage_manager = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_PTR(storage_manager)

	FHUDGoldUpdate callback;
	callback.BindUFunction(this, FName("RefreshGoldAmount"));
	storage_manager->InitUpdateGoldCallback(callback);
}

void UInventoryGrid::RefreshGoldAmount(const int32& gold_amount)
{
	CHECK_INVALID_PTR(GoldAmount)
	FString string = UKismetTextLibrary::AsCurrency_Integer(gold_amount, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 3).ToString();
	string.RemoveAt(0);

	GoldAmount->SetText(FText::FromString(string));
}

void UInventoryGrid::OnClickedGoldInteraction()
{
	UModalPopUp* popup_widget = CreateWidget<UModalPopUp>(GetOwningPlayer(), m_modal_class);
	CHECK_INVALID_PTR(popup_widget)
	popup_widget->InitModal(FString("Drop Gold"));

	m_ui_mgr->RegistUI(popup_widget);
}
