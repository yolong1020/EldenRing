// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventorySlotGrid.h"
#include "HUD/Inventory/InventorySlot.h"
#include "HUD/Inventory/InventoryComponent.h"
#include "HUD/Inventory/InventoryItem.h"
#include "HUD/Structure/InventoryLine.h"
#include "HUD/Structure/InventoryTile.h"
#include "Items/ItemObject.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Basic/DebugMacros.h"

void UInventorySlotGrid::InitGridSlotWidget(UInventoryComponent* inventory_component, const int32& colum_count, const int32& row_count)
{
	m_inventory_component	= inventory_component;
	m_colums				= colum_count;
	m_rows					= row_count;

	FVector2D	resolution	= FVector2D(GSystemResolution.ResX, GSystemResolution.ResY);
	int32		gird_x		= int32((resolution.X * 0.5) * 0.6);
	int32		gird_y		= int32(gird_x * 1.32);
	m_border_size			= FVector2D(gird_x, gird_y);

	CHECK_INVALID(GridBorder)
	UCanvasPanelSlot* border_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	CHECK_INVALID(border_slot)
	border_slot->SetSize(m_border_size);
	border_slot->SetPosition(FVector2D(0, 0));

	m_border_position = border_slot->GetPosition();

	CHECK_INVALID(GridCanvasPanel)
	UBorderSlot* grid_canvas_slot = UWidgetLayoutLibrary::SlotAsBorderSlot(GridCanvasPanel);
	CHECK_INVALID(grid_canvas_slot)
	FMargin margin	= grid_canvas_slot->GetPadding();
	double	size	= ((gird_x - (margin.Left + margin.Right)) / colum_count);

	for (int idx_col = 0; idx_col < m_colums; ++idx_col)
	{
		for (int idx_row = 0; idx_row < m_rows; ++idx_row)
		{
			UInventorySlot* item_widget = CreateWidget<UInventorySlot>(GetOwningPlayer(), m_widget_class);
			CHECK_INVALID(item_widget)
			item_widget->InitSlotWidget(size);

			UPanelSlot*			panel_slot			= GridCanvasPanel->AddChild(item_widget);
			UCanvasPanelSlot*	canvas_panel_slot	= Cast<UCanvasPanelSlot>(panel_slot);

			canvas_panel_slot->SetSize(FVector2D(size, size));
			canvas_panel_slot->SetPosition(FVector2D(idx_col * size, idx_row * size));
		}
	}
}
