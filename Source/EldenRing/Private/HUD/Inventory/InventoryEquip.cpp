// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryEquip.h"
#include "HUD/Inventory/InventoryEquipSlot.h"
#include "HUD/Inventory/InventoryComponent.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Items/ItemObject.h"
#include "Items/Equipment/EquipmentTypes.h"
#include "Basic/DebugMacros.h"

void UInventoryEquip::InitEquipWidget(UInventoryComponent* const inventory_component, const float& tile_size)
{
	m_inventory_component = inventory_component;
	CHECK_INVALID(m_inventory_component);

	FItemOnAdd			add_delegate		= m_inventory_component->GetItemAddDelegate();
	FItemOnAddAtEmpty	add_empty_delegate	= m_inventory_component->GetItemAddAtEmptyDelegate();
	FItemOnRemoved		remove_delegate		= m_inventory_component->GetItemRemoveDelegate();

	FVector2D	resolution = FVector2D(GSystemResolution.ResX, GSystemResolution.ResY);
	int32		gird_x = int32((resolution.X * 0.5) * 0.6);
	int32		gird_y = int32(gird_x * 1.32);

	CHECK_INVALID(GridBorder)
	UCanvasPanelSlot* border_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	CHECK_INVALID(border_slot)
	border_slot->SetSize(FVector2D(gird_x, gird_y));
	border_slot->SetPosition(FVector2D(0, 0));

	CHECK_INVALID(HeadSlot)
	HeadSlot->InitSlotWidget(this, add_delegate, add_empty_delegate, remove_delegate, EEquipmentType::EET_Head, tile_size, m_slot_size_head);

	CHECK_INVALID(BodySlot)
	BodySlot->InitSlotWidget(this, add_delegate, add_empty_delegate, remove_delegate, EEquipmentType::EET_Body, tile_size, m_slot_size_body);

	CHECK_INVALID(HandsSlot)
	HandsSlot->InitSlotWidget(this, add_delegate, add_empty_delegate, remove_delegate, EEquipmentType::EET_Arms, tile_size, m_slot_size_hands);

	CHECK_INVALID(PantsSlot)
	PantsSlot->InitSlotWidget(this, add_delegate, add_empty_delegate, remove_delegate, EEquipmentType::EET_Legs, tile_size, m_slot_size_pants);

	CHECK_INVALID(RightWeaponSlot)
	RightWeaponSlot->InitSlotWidget(this, add_delegate, add_empty_delegate, remove_delegate, EEquipmentType::EET_Weapon, tile_size, m_slot_size_weapon);

	CHECK_INVALID(LeftWeaponSlot)
	LeftWeaponSlot->InitSlotWidget(this, add_delegate, add_empty_delegate, remove_delegate, EEquipmentType::EET_Weapon, tile_size, m_slot_size_weapon);
}

UInventoryEquipSlot* const UInventoryEquip::GetSlotWidget(const EEquipmentType& equip_type)
{
	switch (equip_type)
	{
	case EEquipmentType::EET_Head: return HeadSlot;
	case EEquipmentType::EET_Body: return BodySlot;
	case EEquipmentType::EET_Arms: return HandsSlot;
	case EEquipmentType::EET_Legs: return PantsSlot;
	}

	return nullptr;
}

UInventoryEquipSlot* const UInventoryEquip::GetEmptyWeaponSlotWidget()
{
	if (nullptr != RightWeaponSlot && RightWeaponSlot->IsEmpty())
	{
		return RightWeaponSlot;
	}
	else if (nullptr != LeftWeaponSlot && LeftWeaponSlot->IsEmpty())
	{
		return LeftWeaponSlot;
	}

	return nullptr;
}

UInventoryEquipSlot* const UInventoryEquip::GetMatchingItemSlot(const EEquipmentType& equip_type, UItemObject* const item_object)
{
	if (equip_type == EEquipmentType::EET_None || equip_type == EEquipmentType::EET_Max) { return nullptr; }

	if (equip_type == EEquipmentType::EET_Weapon)
	{
		if (nullptr != RightWeaponSlot && RightWeaponSlot->IsMatching(item_object))		{ return RightWeaponSlot; }
		else if (nullptr != LeftWeaponSlot && LeftWeaponSlot->IsMatching(item_object))	{ return LeftWeaponSlot; }
	}
	else 
	{
		UInventoryEquipSlot* slot = GetSlotWidget(equip_type);
		if (nullptr != slot && slot->IsMatching(item_object)) { return slot; }
	}

	return nullptr;
}

void UInventoryEquip::CheckAndUnequipSlot(UItemObject* const item_object)
{
	CHECK_INVALID(item_object)

	EEquipmentType equip_type = item_object->GetItemEquipType();
	if (equip_type == EEquipmentType::EET_None || equip_type == EEquipmentType::EET_Max) { return; }

	UInventoryEquipSlot* slot = nullptr;

	if (EEquipmentType::EET_Weapon == equip_type)
	{
		if		(nullptr != RightWeaponSlot && RightWeaponSlot->IsMatching(item_object))	{ slot = RightWeaponSlot; }
		else if (nullptr != LeftWeaponSlot && LeftWeaponSlot->IsMatching(item_object))		{ slot = LeftWeaponSlot; }
	}
	else { slot = GetSlotWidget(equip_type); }

	// 스왑인지 확인할 수 있는 수단이 필요하다.
	CHECK_INVALID(slot)
	slot->UnequipItem();
}

bool UInventoryEquip::IsSlotEmpty(const EEquipmentType& equip_type)
{
	if (EEquipmentType::EET_None == equip_type || EEquipmentType::EET_Max == equip_type) { return false; }
	
	UInventoryEquipSlot* slot = nullptr;
	switch (equip_type)
	{
		case EEquipmentType::EET_Head: slot = HeadSlot;  break;
		case EEquipmentType::EET_Body: slot = BodySlot;  break;
		case EEquipmentType::EET_Arms: slot = HandsSlot; break;
		case EEquipmentType::EET_Legs: slot = PantsSlot; break;
		case EEquipmentType::EET_Weapon:
		{
			bool is_empty;
			if (nullptr != RightWeaponSlot)	
			{
				is_empty = RightWeaponSlot->IsEmpty(); 
				if (is_empty) { return true; }
			}
			else if (nullptr != LeftWeaponSlot) 
			{ 
				is_empty = LeftWeaponSlot->IsEmpty();
				if (is_empty) { return true; }
			}
		}
		break;
	}

	if (nullptr == slot) { return false; }

	return slot->IsEmpty();
}
