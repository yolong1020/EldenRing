// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryComponent.h"
#include "HUD/Inventory/Inventory.h"
#include "HUD/Inventory/InventoryGrid.h"
#include "HUD/Inventory/InventoryEquip.h"
#include "HUD/Inventory/InventoryEquipSlot.h"
#include "HUD/Structure/InventoryTile.h"
#include "GameFramework/Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Items/ItemObject.h"
#include "Kismet/GameplayStatics.h"
#include "GameState/BaseGameState.h"
#include "Basic/DebugMacros.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CHECK_INVALID(m_widget_inventory_class)

	ACharacter* actor = Cast<ACharacter>(GetOwner());
	CHECK_INVALID(actor)

	m_player_controller = Cast<APlayerController>(actor->GetController());
	CHECK_INVALID(m_player_controller)

	m_widget_inventory = CreateWidget<UInventory>(m_player_controller, m_widget_inventory_class);
	CHECK_INVALID(m_widget_inventory)

	UWidget* widget_grid	= m_widget_inventory->GetWidgetFromName(FName("WBP_Inventory_Gird"));
	m_widget_grid		= Cast<UInventoryGrid>(widget_grid);

	UWidget* widget_equip	= m_widget_inventory->GetWidgetFromName(FName("WBP_Inventory_Equip"));
	m_widget_equipment	= Cast<UInventoryEquip>(widget_equip);

	ItemOnRemoved	 = ItemOnRemoved.CreateUFunction(this, FName("RemoveItem"));
	ItemOnAdd	 = ItemOnAdd.CreateUFunction(this, FName("AddItemAtTile"));
	ItemOnAddAtEmpty = ItemOnAddAtEmpty.CreateUFunction(this, FName("AddItemAtEmptySlot"));

	CHECK_INVALID(m_widget_grid)
	m_widget_grid->InitGridWidget(this, m_widget_inventory->GetTileSize(), m_colum_count, m_row_count);

	m_slots.SetNum(m_colum_count * m_row_count);

	CHECK_INVALID(m_widget_equipment)
	m_widget_equipment->InitEquipWidget(this, m_widget_inventory->GetTileSize());
}

void UInventoryComponent::AddItem(UItemObject* item_object, const int32& index)
{
	const FIntPoint point		= item_object->GetDimension();
	FInventoryTile  start_tile	= IndexToTile(index);

	int32		find_idx;
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

			item_object->SetCurrentHUD(EDragAndDropHUD::EDAD_Inventory);
			item_object->SetLastInventoryLocation(start_tile);
			m_slots[find_idx] = item_object;
		}
	}

	if (m_is_opened) { m_widget_grid->RefreshGridWidget(); }
}

FInventoryTile UInventoryComponent::IndexToTile(int32 index)
{
	return FInventoryTile(int32(index % m_colum_count), int32(index / m_colum_count));
}

int32 UInventoryComponent::TileToIndex(const FInventoryTile& tile)
{
	return tile.X + (tile.Y * m_colum_count);
}

void UInventoryComponent::RemoveItem(UItemObject* item_object)
{
	CHECK_INVALID(item_object)

	int32 slot_count = m_slots.Num();
	for (int i = 0; i < slot_count; ++i)
	{
		if (false == m_slots.IsValidIndex(i)) continue;

		if (m_slots[i] == item_object) m_slots[i] = nullptr;
	}

	m_widget_grid->RefreshGridWidget();
}

void UInventoryComponent::AddItemAtTile(UItemObject* item_object, const FInventoryTile& start_tile)
{
	AddItem(item_object, TileToIndex(start_tile));
}

void UInventoryComponent::AddItemAtEmptySlot(UItemObject* item_object)
{
	if (false == TryAddItem(item_object))
	{
		ABaseGameState* game_state = Cast<ABaseGameState>(UGameplayStatics::GetGameState(this));
		if (nullptr == game_state) return;

		game_state->SpawnItemFromActor(GetOwner(), item_object, true);
	}
}

void UInventoryComponent::ToggleInventory()
{
	CHECK_INVALID(m_widget_inventory)

	if (m_widget_inventory->IsInViewport())
	{
		m_widget_inventory->RemoveFromParent();

		m_player_controller->bShowMouseCursor = false;
		m_is_opened = false;

		UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_player_controller);
	}
	else
	{
		m_widget_inventory->AddToViewport();
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(m_player_controller, m_widget_inventory);

		m_player_controller->bShowMouseCursor = true;
		m_widget_grid->RefreshGridWidget();
		m_is_opened = true;
	}
}

bool UInventoryComponent::TryAddItem(UItemObject* item_object)
{
	if (nullptr == item_object || nullptr == &m_slots) return false;

	int32 index = 0;
	for (const UItemObject* slot : m_slots)
	{
		if (false == IsRoomAvailable(item_object, index)) { ++index; }
		else
		{
			AddItem(item_object, index);
			return true;
		}
	}

	//	Rotate Item and Try Again
	item_object->RotateItemObject();

	index = 0;
	for (const UItemObject* slot : m_slots)
	{
		if (false == IsRoomAvailable(item_object, index)) { ++index; }
		else
		{
			AddItem(item_object, index);
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::TryAddItemAt(UItemObject* item_object, const int32& index)
{
	if (nullptr == item_object || false == IsRoomAvailable(item_object, index)) return false;

	AddItem(item_object, index);
	return true;
}

bool UInventoryComponent::TryAddItemAndEquip(UItemObject* item_object)
{
	if (nullptr == item_object || nullptr == m_widget_equipment) return false;

	if (m_widget_equipment->IsSlotEmpty(EEquipmentType::EET_Weapon))
	{
		UInventoryEquipSlot* weapon_slot = m_widget_equipment->GetEmptyWeaponSlotWidget();
		if (nullptr == weapon_slot) return false;

		weapon_slot->EquipItem(item_object);
		return true;
	}

	return false;
}

const TMap<UItemObject*, FInventoryTile> UInventoryComponent::GetAllItems()
{
	TMap<UItemObject*, FInventoryTile> all_items;

	for (int i = 0 ; i < m_slots.Num(); ++i)
	{
		if (nullptr == m_slots[i] || all_items.Contains(m_slots[i])) continue;

		all_items.Add(m_slots[i], IndexToTile(i));
	}

	return all_items;
}

void UInventoryComponent::RefreshGoldAmount(const int32& gold_amount)
{
	m_widget_grid->RefreshGoldAmount(gold_amount);
}

void UInventoryComponent::CheckAndUnequipSlot(UItemObject* const item_object)
{
	m_widget_equipment->CheckAndUnequipSlot(item_object);
}

bool UInventoryComponent::IsRoomAvailableForPayload(UItemObject* item_object)
{
	if (nullptr == item_object || nullptr == m_widget_grid) return false;

	FInventoryTile start_tile_draged = m_widget_grid->GetStartTileDraged();

	return IsRoomAvailable(item_object, TileToIndex(start_tile_draged));
}

bool UInventoryComponent::IsRoomAvailable(UItemObject* item_object, const int32& index)
{
	if (nullptr == item_object) return false;

	const FIntPoint point	    = item_object->GetDimension();
	FInventoryTile  start_tile  = IndexToTile(index);
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

bool UInventoryComponent::IsInInventory(UItemObject* item_object)
{
	for (int i = 0; i < m_slots.Num(); ++i)
	{
		if (nullptr == m_slots[i]) continue;
		if (m_slots[i] == item_object) return true;
	}

	return false;
}

