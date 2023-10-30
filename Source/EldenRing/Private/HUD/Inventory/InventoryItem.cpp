// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryItem.h"
#include "HUD/Inventory/InventoryComponent.h"
#include "HUD/Structure/InventoryTile.h"
#include "Items/ItemObject.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Materials/MaterialInterface.h"
#include "Components/CanvasPanelSlot.h"
#include "Materials/MaterialInterface.h"
#include "Basic/DebugMacros.h"

void UInventoryItem::InitItemWidget(FItemOnRemoved& func, UItemObject* item_object, const float& tile_size)
{
	m_item_object = item_object;
	m_tile_size   = tile_size;

	ItemOnRemoved = func;
	ItemRefresh();
}

void UInventoryItem::InitItemWidget(FItemOnRemoved& func, UItemObject* item_object, const float& tile_size, const FVector2D& custom_size)
{
	m_item_object = item_object;
	m_tile_size   = tile_size;
	
	ItemOnRemoved = func;

	m_slot_size.Set(custom_size.X * m_tile_size, custom_size.Y * m_tile_size);
	EquipedItemRefresh();
}

FReply UInventoryItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UInventoryItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	m_widget_drag = UWidgetBlueprintLibrary::CreateDragDropOperation(m_drag_drop_class);
	m_widget_drag->Payload		 = m_item_object;
	m_widget_drag->DefaultDragVisual = this;
	m_widget_drag->Pivot		 = EDragPivot::CenterCenter;
	m_widget_drag->Offset		 = FVector2D(0, 0);
	OutOperation			 = m_widget_drag;

	ItemOnRemoved.Execute(m_item_object);
	RemoveFromParent();
}

void UInventoryItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	CHECK_INVALID(BGBorder);
	BGBorder->SetBrushColor(FLinearColor(0, 1, 0, 0.25));
}

void UInventoryItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	CHECK_INVALID(BGBorder);
	BGBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.25));
}

void UInventoryItem::Refresh()
{
	CHECK_INVALID(m_item_object)

	BGSizeBox->SetWidthOverride(m_slot_size.X);
	BGSizeBox->SetHeightOverride(m_slot_size.Y);

	UCanvasPanelSlot* canvas = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemImage);
	canvas->SetSize(m_slot_size);

	bool is_rotated = m_item_object->IsRotated();
	UMaterialInterface* icon = m_item_object->GetIconImage(is_rotated);

	ItemImage->SetBrushFromMaterial(icon);
	ItemImage->SetDesiredSizeOverride(FVector2D(1, 1));
}

void UInventoryItem::ItemRefresh()
{
	FIntPoint dimension = m_item_object->GetDimension();
	m_slot_size.Set(dimension.X * m_tile_size, dimension.Y * m_tile_size);

	Refresh();
}

void UInventoryItem::EquipedItemRefresh()
{
	Refresh();
}
