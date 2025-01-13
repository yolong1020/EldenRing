// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlotGrid.h"
#include "UI/Inventory/InventorySlot.h"
#include "UI/Inventory/InventoryItem.h"
#include "UI/Structure/InventoryLine.h"
#include "UI/Structure/InventoryTile.h"
#include "Items/ItemObject.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

void UInventorySlotGrid::InitGridSlotWidget()
{
	m_size = StorageBoardSize;

	FVector2D	resolution	= FVector2D(GSystemResolution.ResX, GSystemResolution.ResY);
	int32		gird_x		= int32((resolution.X * 0.5) * 0.6);
	int32		gird_y		= int32(gird_x * 1.32);
	m_border_size			= FVector2D(gird_x, gird_y);

	// CHECK_INVALID_PTR(GridBorder)
	// UCanvasPanelSlot* border_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	// CHECK_INVALID_PTR(border_slot)
	// border_slot->SetSize(m_border_size);
	// border_slot->SetPosition(FVector2D(0, 0));

	// m_border_position = border_slot->GetPosition();

	CHECK_INVALID_PTR(GridBorder)
	FMargin margin = GridBorder->GetPadding();
	double	size   = ((gird_x - (margin.Left + margin.Right)) / m_size.X);
	
	for (int idx_row = 0; idx_row < m_size.Y; ++idx_row)
	{
		for (int idx_col = 0; idx_col < m_size.X; ++idx_col)
		{
			UInventorySlot* item_widget = CreateWidget<UInventorySlot>(GetOwningPlayer(), m_widget_class);
			CHECK_INVALID_PTR(item_widget)
			item_widget->InitSlotWidget(size);
	
			UPanelSlot*			panel_slot	= UniformGrid->AddChild(item_widget);
			UUniformGridSlot*	grid_slot	= Cast<UUniformGridSlot>(panel_slot);

			grid_slot->SetColumn(idx_col);
			grid_slot->SetRow(idx_row);
		}
	}
}
