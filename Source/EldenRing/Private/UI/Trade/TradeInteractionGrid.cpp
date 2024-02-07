// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trade/TradeInteractionGrid.h"
#include "UI/Trade/TradeItem.h"
#include "UI/Inventory/InventoryItem.h"
#include "UI/PopUp/PopUp.h"
#include "UI/PopUp/PopUpTypes.h"
#include "Items/ItemObject.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameBase/ERGameState.h"
#include "Slate/SlateBrushAsset.h"
#include "System/StorageManager.h"
#include "System/TradeManager.h"
#include "Characters/Players/C0000.h"
#include "Characters/NPC/Vender/Vender.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

void UTradeInteractionGrid::InitInteractionGridWidget(const bool IsPlayer, const FMargin& margin)
{
	m_is_player = IsPlayer;

	FIntVector2 board = StorageBoardSize;
	FVector2D widget_size = FVector2D(SizeBox->GetWidthOverride(), SizeBox->GetHeightOverride());

	m_colum		= board.X;
	m_tile_size	= widget_size.X / m_colum;
	double w_space	= (widget_size.X - (m_tile_size * board.X)) / (board.X - 1);
	double h_space	= (widget_size.Y - (m_tile_size * board.Y)) / (board.Y - 1);
	m_space_size	= FVector2D(w_space, h_space);

	FRefreshInteractionGrid callback;
	callback.BindUFunction(this, FName("RefreshGridWidget"));
	m_trade_mgr->InitUpdateGridCallback(IsPlayer, callback);
}

void UTradeInteractionGrid::RefreshGridWidget()
{
	CHECK_INVALID_PTR(Canvas)
	Canvas->ClearChildren();

	TArray<TObjectPtr<UItemObject>>* slots = m_is_player ? m_storage_mgr->GetSlots() : m_trade_mgr->GetVenderSlots();
	TSet<TObjectPtr<UItemObject>> all_items;
	for (int i = 0; i < slots->Num(); ++i)
	{
		if (nullptr == (*slots)[i] || all_items.Contains((*slots)[i])) continue;

		FInventoryTile tile(int32(i % m_colum), int32(i / m_colum));
		
		UTradeItem* item_widget  = CreateWidget<UTradeItem>(GetOwningPlayer(), m_widget_class);
		CHECK_INVALID_PTR(item_widget)
		item_widget->InitItemWidget(m_is_player, (*slots)[i], m_tile_size);

		FIntPoint dimension = (*slots)[i]->GetDimension();
		FVector2D size = FVector2D(dimension.X * m_tile_size, dimension.Y * m_tile_size);
		if (dimension.X > 1) { size.X += (m_space_size.X * (dimension.X - 1)); }
		if (dimension.Y > 1) { size.Y += (m_space_size.Y * (dimension.Y - 1)); }

		FVector2D position = FVector2D(((double)tile.X * m_tile_size), ((double)tile.Y * m_tile_size));
		if (tile.X > 0) { position.X += (m_space_size.X * tile.X); }
		if (tile.Y > 0) { position.Y += (m_space_size.Y * tile.Y); }

		UPanelSlot*	  panel_slot  = Canvas->AddChild(item_widget);
		UCanvasPanelSlot* canvas_slot = Cast<UCanvasPanelSlot>(panel_slot);

		canvas_slot->SetSize(size);
		canvas_slot->SetPosition(position);

		all_items.Add((*slots)[i]);
	}
}

void UTradeInteractionGrid::GetMousePositionInTile(const FVector2d& mouse_position, bool& is_right, bool& is_down)
{
	is_right = (UKismetMathLibrary::Divide_DoubleDouble(mouse_position.X, m_tile_size)) > (m_tile_size * 0.5);
	is_down  = (UKismetMathLibrary::Divide_DoubleDouble(mouse_position.Y, m_tile_size)) > (m_tile_size * 0.5);
}

const TObjectPtr<UItemObject> UTradeInteractionGrid::GetPayload(UDragDropOperation* operation)
{
	if (nullptr == operation) { return nullptr; }

	return Cast<UItemObject>(operation->Payload);
}

void UTradeInteractionGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	m_storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_trade_mgr = instance->GetSubsystem<UTradeManager>();
	CHECK_INVALID_SMART_PTR(m_trade_mgr)

	m_brush = LoadObject<USlateBrushAsset>(nullptr, TEXT("DataTable'/Game/Blueprint/UI/Inventory/SB_InventoryColor.SB_InventoryColor'"), nullptr, LOAD_None, nullptr);
	CHECK_INVALID_PTR(m_brush)

	m_widget_class = LoadClass<UTradeItem>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Trade/Item/WBP_TradeItem.WBP_TradeItem_C'"));
	CHECK_INVALID_PTR(m_widget_class)
}

int32 UTradeInteractionGrid::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 result = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FPaintContext paint_context = FPaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	TArray<TObjectPtr<UItemObject>>* slots = m_storage_mgr->GetSlots();
	if (slots == nullptr) return result;

	bool drag_drop_result = UWidgetBlueprintLibrary::IsDragDropping();
	if (false == m_draw_drop_location || false == drag_drop_result) return result;

	TObjectPtr<UItemObject> payload = GetPayload(UWidgetBlueprintLibrary::GetDragDroppingContent());
	if (nullptr == payload) return result;

	FIntPoint dimension = payload->GetDimension();
	FVector2D size = FVector2D(dimension.X * m_tile_size, dimension.Y * m_tile_size);
	if (dimension.X > 1) { size.X += (m_space_size.X * (dimension.X - 1)); }
	if (dimension.Y > 1) { size.Y += (m_space_size.Y * (dimension.Y - 1)); }

	FVector2D position = FVector2D(((double)m_start_tile_cursor.X * m_tile_size), ((double)m_start_tile_cursor.Y * m_tile_size));
	if (m_start_tile_cursor.X > 0) { position.X += (m_space_size.X * m_start_tile_cursor.X); }
	if (m_start_tile_cursor.Y > 0) { position.Y += (m_space_size.Y * m_start_tile_cursor.Y); }

	bool is_enable = m_storage_mgr->IsEmptyTile(payload, m_start_tile_cursor);
	FLinearColor color = is_enable ? FLinearColor(0, 1, 0, 0.25) : FLinearColor(1, 0, 0, 0.25);

	UWidgetBlueprintLibrary::DrawBox(paint_context, position, size, m_brush, color);

	return result;
}

bool UTradeInteractionGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	TObjectPtr<UItemObject> payload = GetPayload(InOperation);
	if (nullptr == payload) return result;

	EDragAndDropHUD current = payload->GetCurrentHUD();
	if (!m_is_player && EDragAndDropHUD::EDAD_Inventory == current) 
	{
		UPopUp* popup = m_trade_mgr->GetPopup();
		if (nullptr == popup) 
		{
			m_draw_drop_location = false;
			return result;
		}

		FPopupActiveBtn Active;
		Active.BindLambda([&, payload]()->void {
				AC0000*		player  = GetOwningPlayerPawn<AC0000>();
				AGameCharacter* vender	= Cast<AGameCharacter>(m_trade_mgr->GetVender());

				const FItemData* data = payload->GetItemData();
				CHECK_INVALID_PTR(data)

				if (nullptr != data && player && vender)
				{
					m_trade_mgr->TryAddItem(false, payload);
					m_trade_mgr->TryRemoveItem(true, payload);

					player->SetGold(data->Cost * 0.5);
					vender->SetGold(-(data->Cost * 0.5));

					m_trade_mgr->TryUpdateGold(true, player->GetTotalGold());
					m_trade_mgr->TryUpdateGold(false, vender->GetTotalGold());

					TArray<TObjectPtr<UItemObject>>* items = Cast<AVender>(vender)->GetTradeItem();
					items->Add(payload);
				}
			});
		FPopupCancelBtn Cancle;
		Cancle.BindLambda([&, payload]()->void { m_trade_mgr->TryAddItem(true, payload); });
		
		popup->UpdataTwoButtonForTrade(EPopupType::EPT_Sell, payload, Active, Cancle);
	}
	else if (m_trade_mgr->TryAddItemAtTile(m_is_player, payload, m_start_tile_cursor))
	{

	}

	m_draw_drop_location = false;
	return result;
}

bool UTradeInteractionGrid::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool drag_over_result = Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	FVector2D event_position = UKismetInputLibrary::PointerEvent_GetScreenSpacePosition(InDragDropEvent);
	FVector2D mouse_position = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, event_position);

	bool is_right = false;
	bool is_down  = false;
	GetMousePositionInTile(mouse_position, is_right, is_down);

	TObjectPtr<UItemObject> item_object = GetPayload(InOperation);
	if (nullptr == item_object) return drag_over_result;

	FIntPoint dimension = item_object->GetDimension();

	int32 x = is_right ? (dimension.X - 1) : dimension.X;
	int32 y = is_down ? (dimension.Y - 1) : dimension.Y;
	x = FMath::Clamp(x, 0, x);
	y = FMath::Clamp(y, 0, y);

	dimension		= UKismetMathLibrary::Divide_IntPointInt(FIntPoint(x, y), 2);
	mouse_position		= UKismetMathLibrary::Divide_Vector2DFloat(mouse_position, m_tile_size);
	FIntPoint tile_pos	= UKismetMathLibrary::Subtract_IntPointIntPoint(FIntPoint(mouse_position.X, mouse_position.Y), dimension);

	m_start_tile_cursor = FInventoryTile(tile_pos.X, tile_pos.Y);

	return true;
}

void UTradeInteractionGrid::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	m_draw_drop_location = true;
}

void UTradeInteractionGrid::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	m_draw_drop_location = false;
}

FReply UTradeInteractionGrid::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}
