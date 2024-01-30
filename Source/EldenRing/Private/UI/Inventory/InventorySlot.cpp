// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlot.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Macro/DebugMacros.h"

void UInventorySlot::InitSlotWidget(const float& slot_size)
{
	CHECK_INVALID_PTR(SlotBorder)
	USizeBoxSlot* slot = UWidgetLayoutLibrary::SlotAsSizeBoxSlot(SlotBorder);
	CHECK_INVALID_PTR(slot)

	SizeBox->SetWidthOverride(slot_size);
	SizeBox->SetHeightOverride(slot_size);
	//slot->SetSize(FVector2D(slot_size, slot_size));
	//slot->SetPosition(FVector2D(0 * slot_size, 0 * slot_size));
}
