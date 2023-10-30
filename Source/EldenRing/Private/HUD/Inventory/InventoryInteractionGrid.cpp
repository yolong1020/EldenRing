// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryInteractionGrid.h"
#include "HUD/Inventory/InventoryComponent.h"
#include "HUD/Inventory/InventoryItem.h"
#include "Items/ItemObject.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/BaseGameState.h"
#include "Basic/DebugMacros.h"

void UInventoryInteractionGrid::InitInteractionGridWidget(UInventoryComponent* inventory_component, const int32& colum_count, const int32& row_count, const FMargin& margin)
{
	m_inventory_component		= inventory_component;

	FVector2D	resolution	= FVector2D(GSystemResolution.ResX, GSystemResolution.ResY);
	int32		gird_x		= int32((resolution.X * 0.5) * 0.6);
	int32		gird_y		= int32(gird_x * 1.32);

	CHECK_INVALID(GridCanvasPanel)
	UBorderSlot* grid_canvas_slot = UWidgetLayoutLibrary::SlotAsBorderSlot(GridCanvasPanel);
	CHECK_INVALID(grid_canvas_slot)

	CHECK_INVALID(GridBorder)
	UCanvasPanelSlot* border_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	CHECK_INVALID(border_slot)

	border_slot->SetSize(FVector2D(gird_x - (margin.Left + margin.Right), gird_y - (margin.Top + margin.Bottom)));
	m_tile_size = ((gird_x - (margin.Left + margin.Right)) / colum_count);
}

void UInventoryInteractionGrid::RefreshGridWidget()
{
	CHECK_INVALID(GridCanvasPanel)
	GridCanvasPanel->ClearChildren();

	CHECK_INVALID(m_inventory_component)
	TMap<UItemObject*, FInventoryTile> items = m_inventory_component->GetAllItems();

	TArray<UItemObject*> keys;
	items.GetKeys(keys);

	for (UItemObject* item : keys)
	{
		FInventoryTile* tile = items.Find(item);
		if (nullptr != tile)
		{
			UInventoryItem* item_widget = CreateWidget<UInventoryItem>(GetOwningPlayer(), m_widget_class);
			CHECK_INVALID(item_widget)
			item_widget->InitItemWidget(m_inventory_component->GetItemRemoveDelegate(), item, m_tile_size);

			UPanelSlot*	  panel_slot		= GridCanvasPanel->AddChild(item_widget);
			UCanvasPanelSlot* canvas_panel_slot	= Cast<UCanvasPanelSlot>(panel_slot);

			canvas_panel_slot->SetAutoSize(true);
			canvas_panel_slot->SetPosition(FVector2D(tile->X * m_tile_size, tile->Y * m_tile_size));
		}
	}
}

void UInventoryInteractionGrid::GetMousePositionInTile(const FVector2d& mouse_position, bool& is_right, bool& is_down)
{
	is_right = (UKismetMathLibrary::Divide_DoubleDouble(mouse_position.X, m_tile_size)) > (m_tile_size * 0.5);
	is_down  = (UKismetMathLibrary::Divide_DoubleDouble(mouse_position.Y, m_tile_size)) > (m_tile_size * 0.5);
}

UItemObject* UInventoryInteractionGrid::GetPayload(UDragDropOperation* operation)
{
	if (nullptr == operation) { return nullptr; }

	return Cast<UItemObject>(operation->Payload);
}

int32 UInventoryInteractionGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 paint_result = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FPaintContext	  paint_context	= FPaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	USlateBrushAsset* brush		= LoadObject<USlateBrushAsset>(nullptr, TEXT("DataTable'/Game/Blueprint/HUD/Inventory/SB_InventoryColor.SB_InventoryColor'"), nullptr, LOAD_None, nullptr);

	// Draw Enable Box
	bool drag_drop_result = UWidgetBlueprintLibrary::IsDragDropping();
	if (false == m_draw_drop_location || false == drag_drop_result) return paint_result;

	UItemObject* payload = GetPayload(UWidgetBlueprintLibrary::GetDragDroppingContent());
	if (nullptr == payload) return paint_result;

	FIntPoint dimension	= payload->GetDimension() * m_tile_size;
	FVector2D size		= FVector2D(dimension.X, dimension.Y);
	FVector2D position	= FVector2D(m_start_tile_draged.X * m_tile_size, m_start_tile_draged.Y * m_tile_size);

	if (nullptr == m_inventory_component) return paint_result;
	bool is_enable_stack = m_inventory_component->IsRoomAvailableForPayload(payload);
	FLinearColor color   = is_enable_stack ? FLinearColor(0, 1, 0, 0.25) : FLinearColor(1, 0, 0, 0.25);

	UWidgetBlueprintLibrary::DrawBox(paint_context, position, size, brush, color);

	return paint_result;
}

bool UInventoryInteractionGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool drop_result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemObject* payload = GetPayload(InOperation);
	if (nullptr == payload) return false;

	if (nullptr == m_inventory_component) { return drop_result; }

	int32 index		= m_inventory_component->TileToIndex(m_start_tile_draged);
	bool  is_add_success	= m_inventory_component->TryAddItemAt(payload, index);

	//	슬롯 회전 후 재시도
	is_add_success = (false == is_add_success) ? m_inventory_component->TryAddItem(payload) : true;

	if (is_add_success) { payload->SetLastInventoryLocation(m_start_tile_draged); }
	else
	{
		//	인벤토리 공간 부족
		ABaseGameState* game_state = Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
		if (nullptr == game_state) return false;

		game_state->SpawnItemFromActor(m_inventory_component->GetOwner(), payload, true);
	}

	return true;
}

bool UInventoryInteractionGrid::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool drag_over_result	 = Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	FVector2D event_position = UKismetInputLibrary::PointerEvent_GetScreenSpacePosition(InDragDropEvent);
	FVector2D mouse_position = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, event_position);

	bool is_right	= false;
	bool is_down	= false;
	GetMousePositionInTile(mouse_position, is_right, is_down);

	UItemObject* item_object = GetPayload(InOperation);
	if (nullptr == item_object) return drag_over_result;

	FIntPoint dimension = item_object->GetDimension();

	int32 x = is_right ? (dimension.X - 1) : dimension.X;
	int32 y = is_down ? (dimension.Y - 1) : dimension.Y;
	x = FMath::Clamp(x, 0, x);
	y = FMath::Clamp(y, 0, y);

	dimension		= UKismetMathLibrary::Divide_IntPointInt(FIntPoint(x, y), 2);
	mouse_position		= UKismetMathLibrary::Divide_Vector2DFloat(mouse_position, m_tile_size);
	FIntPoint tile_pos	= UKismetMathLibrary::Subtract_IntPointIntPoint(FIntPoint(mouse_position.X, mouse_position.Y), dimension);

	m_start_tile_draged = FInventoryTile(tile_pos.X, tile_pos.Y);

	return true;
}

void UInventoryInteractionGrid::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	m_draw_drop_location = true;
}

void UInventoryInteractionGrid::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	m_draw_drop_location = false;
}

FReply UInventoryInteractionGrid::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

FReply UInventoryInteractionGrid::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);

	if (EKeys::R == UKismetInputLibrary::GetKey(InKeyEvent))
	{
		UDragDropOperation* drag_operation = UWidgetBlueprintLibrary::GetDragDroppingContent();
		if (nullptr == drag_operation) return FReply::Unhandled();

		UItemObject* payload = GetPayload(drag_operation);
		if (nullptr == payload) return FReply::Unhandled();

		payload->RotateItemObject();

		UInventoryItem* item_widget = Cast<UInventoryItem>(drag_operation->DefaultDragVisual);
		if (nullptr != item_widget) { item_widget->ItemRefresh(); }
	}

	return FReply::Handled();
}
