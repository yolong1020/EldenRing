// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryEquip.h"
#include "UI/Inventory/InventoryEquipSlot.h"
#include "Macro/BasicMacro.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Items/ItemObject.h"
#include "Items/Equipment/EquipmentTypes.h"
#include "Kismet/GameplayStatics.h"
#include "System/EquipmentManager.h"
#include "System/StorageManager.h"
#include "Characters/Players/C0000.h"
#include "Macro/DebugMacros.h"

void UInventoryEquip::InitEquipWidget(AC0000* const Player)
{
	FVector2D	resolution = FVector2D(GSystemResolution.ResX, GSystemResolution.ResY);
	int32		gird_x	   = int32((resolution.X * 0.5) * 0.6);
	int32		gird_y	   = int32(gird_x * 1.32);

	CHECK_INVALID_PTR(GridBorder)
	UCanvasPanelSlot* border_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);
	// CHECK_INVALID_PTR(border_slot)
	// border_slot->SetSize(FVector2D(gird_x, gird_y));
	// border_slot->SetPosition(FVector2D(0, 0));

	float tile_size = StorageSlotSize;
	TArray<UInventoryEquipSlot*> slots;

	CHECK_INVALID_PTR(HeadSlot)
	HeadSlot->InitSlotWidget(EEquipmentType::EET_Head, tile_size, m_slot_size_head);
	slots.Add(HeadSlot);

	CHECK_INVALID_PTR(BodySlot)
	BodySlot->InitSlotWidget(EEquipmentType::EET_Body, tile_size, m_slot_size_body);
	slots.Add(BodySlot);

	CHECK_INVALID_PTR(HandsSlot)
	HandsSlot->InitSlotWidget(EEquipmentType::EET_Arms, tile_size, m_slot_size_hands);
	slots.Add(HandsSlot);

	CHECK_INVALID_PTR(PantsSlot)
	PantsSlot->InitSlotWidget(EEquipmentType::EET_Legs, tile_size, m_slot_size_pants);
	slots.Add(PantsSlot);

	CHECK_INVALID_PTR(RightWeaponSlot)
	RightWeaponSlot->InitSlotWidget(EEquipmentType::EET_Weapon, tile_size, m_slot_size_weapon);
	slots.Add(RightWeaponSlot);

	CHECK_INVALID_PTR(LeftWeaponSlot)
	LeftWeaponSlot->InitSlotWidget(EEquipmentType::EET_Weapon, tile_size, m_slot_size_weapon);
	slots.Add(LeftWeaponSlot);

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	m_storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_equipment_mgr = instance->GetSubsystem<UEquipmentManager>();
	CHECK_INVALID_SMART_PTR(m_equipment_mgr)

	m_equipment_mgr->SetEquipmentData(Player, MoveTemp(slots));
}

bool UInventoryEquip::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	TObjectPtr<UItemObject> item_object = Cast<UItemObject>(InOperation->Payload);
	if (nullptr == item_object) return result;

	if (EDragAndDropHUD::EDAD_Equipment == item_object->GetCurrentHUD() && !item_object->IsEquiped())
	{
		m_equipment_mgr->TryEquipItem(item_object, item_object->GetItemEquipType(), (FString("RightWeaponSlot") == item_object->GetLastEquipSlotName()));
	}
	else if (EDragAndDropHUD::EDAD_Inventory == item_object->GetCurrentHUD())
	{
		m_storage_mgr->TryAddItemAtTile(item_object, item_object->GetLastInventoryLocation());
	}

	return result;
}
