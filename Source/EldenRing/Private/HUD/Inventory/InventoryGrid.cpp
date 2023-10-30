// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryGrid.h"
#include "HUD/Inventory/InventoryComponent.h"
#include "HUD/Inventory/InventorySlotGrid.h"
#include "HUD/Inventory/InventoryInteractionGrid.h"
#include "HUD/Inventory/InventoryItem.h"
#include "HUD/Structure/InventoryLine.h"
#include "HUD/Structure/InventoryTile.h"
#include "HUD/Modal/ModalPopUp.h"
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
#include "GameState/BaseGameState.h"
#include "Characters/Players/C0000.h"
#include "Slate/SlateBrushAsset.h"
#include "Kismet/KismetTextLibrary.h"
#include "Basic/DebugMacros.h"

void UInventoryGrid::InitGridWidget(UInventoryComponent* inventory_component, const float& tile_size, const int32& colum_count, const int32& row_count)
{
	m_tile_size				= tile_size;
	m_inventory_component	= inventory_component;

	UWidget* widget_slot_grid = GetWidgetFromName(FName("WBP_Inventory_SlotGrid"));
	m_widget_slot_grid = Cast<UInventorySlotGrid>(widget_slot_grid);
	CHECK_INVALID(m_widget_slot_grid)
	m_widget_slot_grid->InitGridSlotWidget(inventory_component, colum_count, row_count);
	
	UWidget* widget_Interaction_grid = GetWidgetFromName(FName("WBP_Inventory_InteractionGrid"));
	m_widget_Interaction_grid = Cast<UInventoryInteractionGrid>(widget_Interaction_grid);
	CHECK_INVALID(m_widget_Interaction_grid)
	m_widget_Interaction_grid->InitInteractionGridWidget(inventory_component, colum_count, row_count, m_margin);
	UCanvasPanelSlot* border_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(m_widget_Interaction_grid);
	CHECK_INVALID(border_slot)

	CHECK_INVALID(ButtonGoldInteraction)
	ButtonGoldInteraction->OnClicked.AddDynamic(this, &UInventoryGrid::OnClickedGoldInteraction);
	RefreshGridWidget();
}

void UInventoryGrid::RefreshGridWidget()
{
	m_widget_Interaction_grid->RefreshGridWidget();
}

void UInventoryGrid::RefreshGoldAmount(const int32& gold_amount)
{
	CHECK_INVALID(GoldAmount)
	FString string = UKismetTextLibrary::AsCurrency_Integer(gold_amount, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 3).ToString();
	string.RemoveAt(0);

	GoldAmount->SetText(FText::FromString(string));
}

const FInventoryTile& UInventoryGrid::GetStartTileDraged()
{
	return m_widget_Interaction_grid->GetStartTileDraged();
}

void UInventoryGrid::OnClickedGoldInteraction()
{
	UModalPopUp* popup_widget = CreateWidget<UModalPopUp>(GetOwningPlayer(), m_modal_class);
	CHECK_INVALID(popup_widget)
	popup_widget->InitModal(FString("Drop Gold"));

	popup_widget->AddToViewport();
}
