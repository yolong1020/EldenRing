// Fill out your copyright notice in the Description page of Project Settings.


#include "System/TradeManager.h"
#include "System/UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Item/ItemInfo.h"
#include "UI/PopUp/PopUp.h"
#include "UI/Trade/Trade.h"
#include "UI/Trade/TradeGrid.h"
#include "Characters/NPC/Vender/Vender.h"
#include "Characters/GameCharacter_Interface.h"
#include "Items/ItemObject.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

void UTradeManager::OpenTrade(const TObjectPtr<APlayerController>& Controller)
{
	CHECK_INVALID_PTR(Controller)
	m_controller = Controller;

	m_ui_mgr->RegistUI(m_widget_trade);
	m_widget_trade->SetController(Controller);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller.Get(), m_widget_trade);

	m_controller->bShowMouseCursor = true;

	IGameCharacter_Interface* character_interface = Cast<IGameCharacter_Interface>(Controller->AcknowledgedPawn);
	CHECK_INVALID_PTR(character_interface)

	TryUpdateStorage();
	TryUpdateGold(true, character_interface->GetTotalGold());
	TryUpdateGold(false, m_vender->GetTotalGold());

	m_vender_slots.Reset();
	m_vender_slots.SetNum(m_colum * m_row, false);

	TArray<TObjectPtr<UItemObject>>* items = m_vender->GetTradeItem();
	for (int i = 0; i < items->Num(); ++i) 
	{
		TryAddItem(false, (*items)[i]);
	}

	m_widget_grid_vender->RefreshOwnerName(m_vender->GetVenderName() + TEXT("의 판매품"));
}

void UTradeManager::InitUpdateGoldCallback(const bool& IsPlayer, const FHUDGoldUpdate& Callback)
{
	FHUDGoldUpdate& callback = IsPlayer ? m_update_gold_player : m_update_gold_vender;

	if (Callback.IsBound()) { callback = Callback; }
}

void UTradeManager::InitUpdateGridCallback(const bool& IsPlayer, const FRefreshInteractionGrid& Callback)
{
	FRefreshInteractionGrid& callback = IsPlayer ? m_update_player : m_update_vender;
	if (Callback.IsBound()) { callback = Callback; }
}

void UTradeManager::InitTradeWidget(TArray<TObjectPtr<UItemObject>>* const PlayerSlots)
{
	m_player_slots = PlayerSlots;

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)

	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)

	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	UClass* widget_class = LoadClass<UTrade>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Trade/TradeGrid/WBP_Trade.WBP_Trade_C'"));
	CHECK_INVALID_PTR(widget_class)
	m_widget_trade = CreateWidget<UTrade>(world, widget_class);

	UWidget* widget_player = m_widget_trade->GetWidgetFromName(FName("WBP_Trade_Gird_Player"));
	m_widget_grid_player = Cast<UTradeGrid>(widget_player);

	UWidget* widget_vender = m_widget_trade->GetWidgetFromName(FName("WBP_Trade_Grid_Partner"));
	m_widget_grid_vender = Cast<UTradeGrid>(widget_vender);

	CHECK_INVALID_PTR(m_widget_grid_player)
	m_widget_grid_player->InitGridWidget(true);

	CHECK_INVALID_PTR(m_widget_grid_vender)
	m_widget_grid_vender->InitGridWidget(false);

	UClass* ItemInfoWidget = LoadClass<UItemInfo>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/ItemInfo/WBP_ItemInfo.WBP_ItemInfo_C'"));
	CHECK_INVALID_PTR(ItemInfoWidget)

	m_item_info = CreateWidget<UItemInfo>(world, ItemInfoWidget);
	CHECK_INVALID_PTR(m_item_info)

	UClass* Popup = LoadClass<UPopUp>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Popup/Common/WBP_Popup.WBP_Popup_C'"));
	CHECK_INVALID_PTR(Popup)

	m_popup = CreateWidget<UPopUp>(world, Popup);
	CHECK_INVALID_PTR(m_popup)
}

void UTradeManager::ShowItemInfo(const FVector2D& Position, const FVector2D& SlotSize, const TObjectPtr<UItemObject> ItemObject)
{
	if (!IsValid(m_item_info) || "None" == m_item_info->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Info is None"))
		return;
	}

	m_item_info->UpdateItemInfo(Position, SlotSize, ItemObject, true);
}

void UTradeManager::HideItemInfo()
{
	if (!IsValid(m_item_info) || "None" == m_item_info->GetName())
	{
			UE_LOG(LogTemp, Warning, TEXT("Item Info is None"))
			return;
	}

	m_item_info->RemoveFromParent();
}

bool UTradeManager::TryUpdateStorage()
{
	if (m_update_player.IsBound() &&
	    m_update_vender.IsBound())
	{
		m_update_player.Execute();
		m_update_vender.Execute();

		return true;
	}

	return false;
}

bool UTradeManager::TryUpdateGold(const bool IsPlayer, const int32& TotalGold)
{
	FHUDGoldUpdate& callback = IsPlayer ? m_update_gold_player : m_update_gold_vender;
	if (!callback.IsBound()) return false;

	callback.Execute(TotalGold);
	return true;
}

bool UTradeManager::TryAddItem(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject)
{
	if (nullptr == ItemObject) return false;

	for (int loop = 0; loop < 2; ++loop) {
		for (int i = 0; i < m_vender_slots.Num(); ++i)
		{
			if (IsEmptyIndex(IsPlayer, ItemObject, i))
			{
				AddItem(IsPlayer, ItemObject, i);
				return true;
			}
		}

		//	Rotate Item and Try Again
		ItemObject->RotateItemObject();
	}

	if (ItemObject->IsRotated()) { ItemObject->RotateItemObject(); }

	return false;
}

bool UTradeManager::TryAddItemAtIndex(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const int32& Index)
{
	if (nullptr == ItemObject || false == IsEmptyIndex(IsPlayer, ItemObject, Index)) return false;

	AddItem(IsPlayer, ItemObject, Index);
	return true;
}

bool UTradeManager::TryAddItemAtTile(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const FInventoryTile& StartTile)
{
	if (nullptr == ItemObject) return false;

	int32 idx = TileToIndex(StartTile);
	for (int i = 0; i < 2; ++i) {

		if (IsEmptyIndex(IsPlayer, ItemObject, idx))
		{
			AddItem(IsPlayer, ItemObject, idx);
			return true;
		}
	}

	return false;
}

bool UTradeManager::TryRemoveItem(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject)
{
	if (nullptr == ItemObject) return false;

	RemoveItem(IsPlayer, ItemObject);
	return true;
}

bool UTradeManager::IsEmptyTile(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const FInventoryTile& Tile)
{
	return IsEmptyIndex(IsPlayer, ItemObject, TileToIndex(Tile));
}

bool UTradeManager::IsEmptyIndex(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const int32& Index)
{
	if (nullptr == ItemObject) return false;

	const FIntPoint point		= ItemObject->GetDimension();
	FInventoryTile  start_tile	= IndexToTile(Index);
	FInventoryTile	check_tile;

	int x_max = start_tile.X + (point.X - 1);
	int y_max = start_tile.Y + (point.Y - 1);

	for (int x = start_tile.X; x <= x_max; ++x)
	{
		for (int y = start_tile.Y; y <= y_max; ++y)
		{
			check_tile.X = x;
			check_tile.Y = y;

			//	Check tile range.
			bool is_vaild_tile = (check_tile.X >= 0 && check_tile.Y >= 0 && check_tile.X < m_colum && check_tile.Y < m_row);
			if (false == is_vaild_tile) return false;

			//	Check index is invalid.
			int32 find_idx = TileToIndex(check_tile);

			bool is_invalid = IsPlayer ? m_player_slots->IsValidIndex(find_idx) : m_vender_slots.IsValidIndex(find_idx);
			if (false == is_invalid) return false;

			if (nullptr != (IsPlayer ? (*m_player_slots)[find_idx] : m_vender_slots[find_idx])) return false;
		}
	}

	return true;
}

void UTradeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FIntVector2 size = StorageBoardSize
	m_colum = size.X;
	m_row	= size.Y;

	m_vender_slots.Reserve(m_colum * m_row);
	m_vender_slots.SetNum(m_colum * m_row, false);
}

const FInventoryTile UTradeManager::IndexToTile(const int32& Index)
{
	return FInventoryTile(int32(Index % m_colum), int32(Index / m_colum));
}

const int32 UTradeManager::TileToIndex(const FInventoryTile& Tile)
{
	return Tile.X + (Tile.Y * m_colum);
}

void UTradeManager::AddItem(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject, const int32& Index)
{
	const FIntPoint point		= ItemObject->GetDimension();
	FInventoryTile  start_tile	= IndexToTile(Index);

	int32			find_idx;
	FInventoryTile	check_tile;

	int x_max = start_tile.X + (point.X - 1);
	int y_max = start_tile.Y + (point.Y - 1);

	for (int x = start_tile.X; x <= x_max; ++x)
	{
		for (int y = start_tile.Y; y <= y_max; ++y)
		{
			check_tile.X = x;
			check_tile.Y = y;

			find_idx = TileToIndex(check_tile);

			ItemObject->SetCurrentHUD(IsPlayer ? EDragAndDropHUD::EDAD_Inventory : EDragAndDropHUD::EDAD_Vender);
			ItemObject->SetLastInventoryLocation(start_tile);
			IsPlayer ? (*m_player_slots)[find_idx] = ItemObject : m_vender_slots[find_idx] = ItemObject;
		}
	}

	if (m_widget_trade->IsInViewport())
	{
		if		(IsPlayer && m_update_player.IsBound())	 { m_update_player.Execute(); }
		else if (!IsPlayer && m_update_vender.IsBound()) { m_update_vender.Execute(); }
	}
}

void UTradeManager::RemoveItem(const bool IsPlayer, const TObjectPtr<UItemObject> ItemObject)
{
	TArray<TObjectPtr<UItemObject>>* slots = IsPlayer ? m_player_slots : &m_vender_slots;
	int32 slot_count = slots->Num();
	for (int i = 0; i < slot_count; ++i)
	{
		if (false == slots->IsValidIndex(i)) continue;
		if ((*slots)[i] == ItemObject) { (*slots)[i] = nullptr; }
	}

	if (!IsPlayer)
	{
		TArray<TObjectPtr<UItemObject>>* items = m_vender->GetTradeItem();
		int32 idx = items->Find(ItemObject);
		if (0 <= idx) { items->RemoveAt(idx); }
	}

	if (m_widget_trade->IsInViewport()) 
	{
		if		 (IsPlayer && m_update_player.IsBound())  { m_update_player.Execute(); }
		else if  (!IsPlayer && m_update_vender.IsBound()) { m_update_vender.Execute(); }
	}
}
