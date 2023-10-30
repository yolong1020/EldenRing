// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventorySlot.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Basic/DebugMacros.h"

void UInventorySlot::InitSlotWidget(const float& slot_size)
{
	CHECK_INVALID(SlotBorder)
	UCanvasPanelSlot* grid_canvas_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlotBorder);
	CHECK_INVALID(grid_canvas_slot)

	grid_canvas_slot->SetSize(FVector2D(slot_size, slot_size));
	grid_canvas_slot->SetPosition(FVector2D(0 * slot_size, 0 * slot_size));
}
