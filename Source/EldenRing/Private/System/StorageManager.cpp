// Fill out your copyright notice in the Description page of Project Settings.


#include "System/StorageManager.h"
#include "System/UIManager.h"
#include "UI/Inventory/Inventory.h"
#include "UI/Inventory/InventoryGrid.h"
#include "UI/Inventory/InventoryEquip.h"
#include "UI/Inventory/InventoryEquipSlot.h"
#include "UI/Item/ItemInfo.h"
#include "Items/ItemObject.h"
#include "Components/PanelWidget.h"
#include "GameBase/ERGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/GameCharacter.h"
#include "Characters/Players/C0000.h"
#include "System/UIManager.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

void UStorageManager::OpenInventory(const IGameCharacter_Interface* Character, const TWeakObjectPtr<APlayerController>& Controller)
{
	if (m_widget_inventory->IsInViewport()) return;

	m_ui_mgr->RegistUI(m_widget_inventory);
	m_widget_inventory->SetController(Controller);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller.Get(), m_widget_inventory.Get());

	Controller->bShowMouseCursor = true;

	TryUpdateStorage();
	TryUpdateGold(Character->GetTotalGold());
}

const bool UStorageManager::IsActiveInventory() const
{
	return m_widget_inventory->IsInViewport();
}

void UStorageManager::InitUpdateGoldCallback(const FHUDGoldUpdate& Callback)
{
	m_update_gold_delegate = Callback;
}

void UStorageManager::InitUpdateGridCallback(const FRefreshInteractionGrid& Callback)
{
	m_update_delegate = Callback;
}

void UStorageManager::InitStorageWidget(AC0000* const Player)
{
	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)

	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)

	UClass* widget_class = LoadClass<UInventory>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Inventory/WBP_Inventory.WBP_Inventory_C'"));
	CHECK_INVALID_PTR(widget_class)

	m_widget_inventory = CreateWidget<UInventory>(GetWorld(), widget_class);
	CHECK_INVALID_PTR(m_widget_inventory)

	UWidget* widget_grid = m_widget_inventory->GetWidgetFromName(FName("WBP_Inventory_Gird"));
	m_widget_grid = Cast<UInventoryGrid>(widget_grid);

	UWidget* widget_equip = m_widget_inventory->GetWidgetFromName(FName("WBP_Inventory_Equip"));
	m_widget_equipment = Cast<UInventoryEquip>(widget_equip);

	CHECK_INVALID_PTR(m_widget_grid)
	m_widget_grid->InitGridWidget();

	CHECK_INVALID_PTR(m_widget_equipment)
	m_widget_equipment->InitEquipWidget(Player);

	if (m_item_info) return;

	UClass* ItemInfoWidget = LoadClass<UItemInfo>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/ItemInfo/WBP_ItemInfo.WBP_ItemInfo_C'"));
	CHECK_INVALID_PTR(ItemInfoWidget)

	m_item_info = CreateWidget<UItemInfo>(GetWorld(), ItemInfoWidget);
	CHECK_INVALID_PTR(m_item_info)
}

void UStorageManager::ShowItemInfo(const FVector2D& Position, const FVector2D& SlotSize, const TObjectPtr<UItemObject> ItemObject)
{
	if (!m_item_info || "None" == m_item_info->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Info is None"))
		return;
	}

	if (m_item_info->IsInViewport()) return;

	m_item_info->UpdateItemInfo(Position, SlotSize, ItemObject, false);
}

void UStorageManager::HideItemInfo()
{
	if (!m_item_info || "None" == m_item_info->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Info is None"))
		return;
	}

	m_item_info->RemoveFromParent();
}

bool UStorageManager::TryUpdateStorage()
{
	if (!m_update_delegate.IsBound()) return false;

	m_update_delegate.Execute();
	return true;
}

bool UStorageManager::TryUpdateGold(const int32& TotalGold)
{
	if (!m_update_gold_delegate.IsBound()) return false;

	m_update_gold_delegate.Execute(TotalGold);
	return true;
}

bool UStorageManager::TryAddItem(const TObjectPtr<UItemObject> ItemObject)
{
	if (!ItemObject) return false;

	for (int loop = 0; loop < 2; ++loop) 
	{
		for (int i = 0; i < m_slots.Num(); ++i)
		{
			if (IsEmptyIndex(ItemObject, i))
			{
				AddItem(ItemObject, i);
				return true;
			}
		}

		//	Rotate Item and Try Again
		ItemObject->RotateItemObject();
	}

	if (ItemObject->IsRotated()) { ItemObject->RotateItemObject(); }

	return false;
}

bool UStorageManager::TryAddItemAtIndex(const TObjectPtr<UItemObject> ItemObject, const int32& Index)
{
	if (nullptr == ItemObject || false == IsEmptyIndex(ItemObject, Index)) return false;

	AddItem(ItemObject, Index);
	return true;
}

bool UStorageManager::TryAddItemAtTile(const TObjectPtr<UItemObject> ItemObject, const FInventoryTile& StartTile)
{
	int32 idx = TileToIndex(StartTile);
	for (int i = 0; i < 2; ++i) {

		if (IsEmptyIndex(ItemObject, idx))
		{
			AddItem(ItemObject, idx);
			return true;
		}
	}

	return false;
}

bool UStorageManager::TryRemoveItem(const TObjectPtr<UItemObject> ItemObject)
{
	if (nullptr == ItemObject) return false;

	RemoveItem(ItemObject);
	return true;
}

void UStorageManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FIntVector2 size = StorageBoardSize
	m_colum_count	= size.X;
	m_row_count		= size.Y;

	m_slots.Reserve(m_colum_count * m_row_count);
	m_slots.SetNum(m_colum_count * m_row_count, false);
}

const FInventoryTile UStorageManager::IndexToTile(const int32& Index) const
{
	return FInventoryTile(int32(Index % m_colum_count), int32(Index / m_colum_count));
}

const int32 UStorageManager::TileToIndex(const FInventoryTile& Tile) const
{
	return Tile.X + (Tile.Y * m_colum_count);
}

void UStorageManager::AddItem(TObjectPtr<UItemObject> ItemObject, const int32& Index)
{
	CHECK_INVALID_PTR(m_widget_inventory)

	const FIntPoint point		= ItemObject->GetDimension();
	FInventoryTile  start_tile  = IndexToTile(Index);

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

			ItemObject->SetCurrentHUD(EDragAndDropHUD::EDAD_Inventory);
			ItemObject->SetLastInventoryLocation(start_tile);
			m_slots[find_idx] = ItemObject;
		}
	}

	if (m_widget_inventory->IsInViewport() && m_update_delegate.IsBound()) { m_update_delegate.Execute(); }
}

void UStorageManager::RemoveItem(const TObjectPtr<UItemObject> ItemObject)
{
	CHECK_INVALID_PTR(m_widget_inventory)

	int32 slot_count = m_slots.Num();
	for (int i = 0; i < slot_count; ++i)
	{
		if (m_slots[i] == ItemObject) m_slots[i] = nullptr;
	}

	if (m_widget_inventory->IsInViewport() && m_update_delegate.IsBound()) { m_update_delegate.Execute(); }
}

void UStorageManager::ClearStorage()
{
	m_slots.Reset();
	m_slots.SetNum(m_colum_count * m_row_count, false);
}

const int32 UStorageManager::HaveSpaceItem(const TObjectPtr<UItemObject> ItemObject) const
{
	if (!ItemObject) return -1;

	for (int loop = 0; loop < 2; ++loop)
	{
		for (int i = 0; i < m_slots.Num(); ++i)
		{
			if (IsEmptyIndex(ItemObject, i))
			{
				return i;
			}
		}

		//	Rotate Item and Try Again
		ItemObject->RotateItemObject();
	}

	if (ItemObject->IsRotated()) { ItemObject->RotateItemObject(); }

	return -1;
}

const bool UStorageManager::IsEmptyTile(const TObjectPtr<UItemObject> ItemObject, const FInventoryTile& Tile) const
{
	return IsEmptyIndex(ItemObject, TileToIndex(Tile));
}

const bool UStorageManager::IsEmptyIndex(const TObjectPtr<UItemObject> ItemObject, const int32& Index) const
{
	if (!ItemObject) return false;
	
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
			bool is_vaild_tile = (check_tile.X >= 0 && check_tile.Y >= 0 && check_tile.X < m_colum_count && check_tile.Y < m_row_count);
			if (false == is_vaild_tile) return false;

			//	Check index is invalid.
			int32 find_idx = TileToIndex(check_tile);
			if (false == m_slots.IsValidIndex(find_idx)) return false;

			if (nullptr != m_slots[find_idx]) return false;
		}
	}

	return true;
}

const bool UStorageManager::IsInInventory(const TObjectPtr<UItemObject> ItemObject) const
{
	if (!ItemObject) return false;

	for (int i = 0; i < m_slots.Num(); ++i)
	{
		if (nullptr == m_slots[i]) continue;
		if (m_slots[i] == ItemObject) return true;
	}

	return false;
}
