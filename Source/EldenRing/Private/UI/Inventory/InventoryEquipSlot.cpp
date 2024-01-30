// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryEquipSlot.h"
#include "UI/Inventory/InventoryEquip.h"
#include "UI/Inventory/InventoryItem.h"
#include "HUD/GameHUD_Actor.h"
#include "HUD/Status/PlayerHUD.h"
#include "System/StorageManager.h"
#include "System/EquipmentManager.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Items/ItemObject.h"
#include "Characters/Players/C0000.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/SkeletalMeshLODModel.h"
#include "Macro/DebugMacros.h"

void UInventoryEquipSlot::InitSlotWidget(const EEquipmentType& equip_type, const float& tile_size, const FVector2D& slot_size)
{
	m_slot_type	= equip_type;
	m_tile_size	= tile_size;

	UMaterialInterface* material = nullptr;
	switch (equip_type)
	{
		case EEquipmentType::EET_Head:	 material = LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/HUD/Equipment/M_Equipment_Slot_Head.M_Equipment_Slot_Head"));		break;
		case EEquipmentType::EET_Body:	 material = LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/HUD/Equipment/M_Equipment_Slot_Body.M_Equipment_Slot_Body"));		break;
		case EEquipmentType::EET_Arms:	 material = LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/HUD/Equipment/M_Equipment_Slot_Hands.M_Equipment_Slot_Hands"));		break;
		case EEquipmentType::EET_Legs:	 material = LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/HUD/Equipment/M_Equipment_Slot_Shoes.M_Equipment_Slot_Shoes"));		break;
		case EEquipmentType::EET_Weapon: material = LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/HUD/Equipment/M_Equipment_Slot_Weapon.M_Equipment_Slot_Weapon"));	break;
		default: return;
	}

	CHECK_INVALID_PTR(material)
	SlotBGImage->SetBrushFromMaterial(material);
}

void UInventoryEquipSlot::EquipItem(const TObjectPtr<UItemObject> ItemObject)
{
	CHECK_INVALID_PTR(ItemObject)

	m_equiped_item = ItemObject;
	m_equiped_item->SetEquipState(true);
	m_equiped_item->SetCurrentHUD(EDragAndDropHUD::EDAD_Equipment);
	if (m_equiped_item->IsRotated()) { m_equiped_item->RotateItemObject(); }

	SlotBorder->ClearChildren();
	m_item_widget = CreateWidget<UInventoryItem>(GetOwningPlayer(), m_widget_class);
	m_item_widget->InitItemWidget(m_equiped_item, FVector2D::ZeroVector, 1, m_slot_size);

	UPanelSlot* panel_slot = SlotBorder->AddChild(m_item_widget);
	SlotBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.25));
}

void UInventoryEquipSlot::UnequipItem()
{
	if (!m_equiped_item) return;

	m_equiped_item->SetEquipState(false);
	m_equiped_item = nullptr;

	SlotBorder->ClearChildren();
}

void UInventoryEquipSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	m_storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_equipment_mgr = instance->GetSubsystem<UEquipmentManager>();
	CHECK_INVALID_SMART_PTR(m_equipment_mgr)
}

bool UInventoryEquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemObject* item_object = Cast<UItemObject>(InOperation->Payload);
	if (!item_object || !SlotBorder) return result;

	if (IsEnableEquipment(item_object))
	{
		// Swap
		if (m_equiped_item && m_equiped_item != item_object)
		{
			SlotBorder->ClearChildren();
			m_storage_mgr->TryAddItem(m_equiped_item);
			m_equipment_mgr->TryUnequipItem(m_equiped_item->GetItemEquipType(), FString("RightWeaponSlot") == GetName());
		}

		m_equipment_mgr->TryEquipItem(item_object, m_slot_type, (FString("RightWeaponSlot") == GetName()));
	}
	else if (EDragAndDropHUD::EDAD_Equipment == item_object->GetCurrentHUD())
	{
		m_equipment_mgr->TryEquipItem(item_object, item_object->GetItemEquipType(), (FString("RightWeaponSlot") == item_object->GetLastEquipSlotName()));
	}

	return result;
}

bool UInventoryEquipSlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool result = Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	TObjectPtr<UItemObject> item_object = Cast<UItemObject>(InOperation->Payload);

	if (nullptr == m_item_widget) return result;

	if (m_equiped_item && item_object == m_equiped_item)
	{
		m_item_widget->InitItemWidget(m_equiped_item, FVector2D::ZeroVector, m_tile_size);

		m_equipment_mgr->TryUnequipItem(m_slot_type, (FString("RightWeaponSlot") == GetName()));
		item_object->SetLastEquipSlotName(GetName());
	}

	return result;
}

void UInventoryEquipSlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	FLinearColor box_color;

	TObjectPtr<UItemObject>  item_object = Cast<UItemObject>(InOperation->Payload);
	if (nullptr == item_object) { box_color = FLinearColor(1, 0, 0, 0.25); }

	bool is_matching = m_slot_type == item_object->GetItemEquipType();
	box_color = is_matching ? FLinearColor(0, 1, 0, 0.25) : FLinearColor(1, 0, 0, 0.25);

	SlotBorder->SetBrushColor(box_color);
}

void UInventoryEquipSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	SlotBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.25));
}

bool UInventoryEquipSlot::IsEnableEquipment(const TObjectPtr<UItemObject> item_object)
{
	EEquipmentType type = item_object->GetItemEquipType();

	if (m_slot_type != type)
	{
		if (EDragAndDropHUD::EDAD_Equipment == item_object->GetCurrentHUD())
		{
			SlotBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.25));
			UInventoryEquipSlot* slot = m_equipment_mgr->GetMatchingSlot(item_object);
			if (nullptr == slot) return false;
			
			slot->EquipItem(*item_object);
			
			return false;
		}
		else
		{
			SlotBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.25));
			m_storage_mgr->TryAddItemAtTile(item_object, item_object->GetLastInventoryLocation());

			return false;
		}
	}

	return true;
}
