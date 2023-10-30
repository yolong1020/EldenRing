// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryEquipSlot.h"
#include "HUD/Inventory/InventoryEquip.h"
#include "HUD/Inventory/InventoryComponent.h"
#include "HUD/Inventory/InventoryItem.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Items/ItemObject.h"
#include "Basic/Public/Characters/Players/C0000.h"
#include "Blueprint/DragDropOperation.h"
#include "Basic/DebugMacros.h"

void UInventoryEquipSlot::InitSlotWidget(UInventoryEquip* const widget_equip, FItemOnAdd& func_add, FItemOnAddAtEmpty& func_add_empty, FItemOnRemoved& func_remove, const EEquipmentType& equip_type, const float& tile_size, const FVector2D& slot_size)
{
	CHECK_INVALID(widget_equip)

	m_inventory_equip	= widget_equip;
	ItemOnAdd			= func_add;
	ItemOnAddEmpty		= func_add_empty;
	ItemOnRemoved		= func_remove;

	m_tile_size			= tile_size;
	m_slot_type			= equip_type;

	FVector2D	resolution			= FVector2D(GSystemResolution.ResX, GSystemResolution.ResY);
	int32		grid_unit			= int32((resolution.X * 0.5) * (11.6 * 0.01));
	FVector2D	slot_bg_size		= FVector2D(slot_size.X * grid_unit, slot_size.Y * grid_unit);
	double		slot_size_offset	= slot_bg_size.X * 0.14;

	m_slot_size	= FVector2D(slot_bg_size.X - slot_size_offset, slot_bg_size.Y - slot_size_offset);

	CHECK_INVALID(SlotBorder)
	UCanvasPanelSlot* canvas_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlotBorder);
	CHECK_INVALID(canvas_slot)
	canvas_slot->SetSize(m_slot_size);

	CHECK_INVALID(SlotBGImage)
	UCanvasPanelSlot* bg_slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlotBGImage);
	CHECK_INVALID(bg_slot)
	bg_slot->SetSize(slot_bg_size);
	bg_slot->SetPosition(FVector2D(0, 0));

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
	CHECK_INVALID(material)
	SlotBGImage->SetBrushFromMaterial(material);
	SlotBGImage->SetDesiredSizeOverride(slot_bg_size);
}

void UInventoryEquipSlot::EquipItem(UItemObject* const item_object)
{
	m_equiped_item = item_object;
	m_equiped_item->SetCurrentHUD(EDragAndDropHUD::EDAD_Equipment);
	if (m_equiped_item->IsRotated()) { m_equiped_item->RotateItemObject(); }

	AC0000* player = Cast<AC0000>(GetOwningPlayerPawn());
	if (nullptr != player) {
		if (EEquipmentType::EET_Weapon == m_slot_type) {
			bool is_right_hand = FString("LeftWeaponSlot") == this->GetName() ? false : true;
			player->EquipWeaponFromInventory(m_equiped_item, is_right_hand);
		}
		else {
			player->EquipArmorFromInventory(m_equiped_item);
		}
	}

	SlotCanvasPanel->ClearChildren();
	m_item_widget = CreateWidget<UInventoryItem>(GetOwningPlayer(), m_widget_class);
	m_item_widget->InitItemWidget(ItemOnRemoved, m_equiped_item, 1, m_slot_size);

	UPanelSlot* panel_slot = SlotCanvasPanel->AddChild(m_item_widget);
	UCanvasPanelSlot* canvas_panel_slot = Cast<UCanvasPanelSlot>(panel_slot);

	canvas_panel_slot->SetAutoSize(true);

	SlotBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.2));
}

void UInventoryEquipSlot::UnequipItem()
{
	m_equiped_item = nullptr;
}

bool UInventoryEquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool drop_result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemObject* item_object = Cast<UItemObject>(InOperation->Payload);

	if (nullptr == item_object		||
		nullptr == SlotCanvasPanel	||
		false   == IsEnableEquipment(item_object)) 
	{ return true; }

	if (nullptr != m_equiped_item) {
		if (item_object != m_equiped_item) {
			SlotCanvasPanel->ClearChildren();
			ItemOnAddEmpty.Execute(m_equiped_item);

			AC0000* player = Cast<AC0000>(GetOwningPlayerPawn());
			if (nullptr == player) { return true; }

			if (EEquipmentType::EET_Weapon == m_slot_type) {
				player->UnequipWeapon(m_slot_type, !(FString("LeftWeaponSlot") == this->GetName()));
			}
			else {
				player->UnequipArmor(m_slot_type);
			}
		}
	}

	EquipItem(item_object);
	return true;
}

bool UInventoryEquipSlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool drag_result = Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	UItemObject* item_object = Cast<UItemObject>(InOperation->Payload);

	if (nullptr == m_item_widget) { return drag_result; }

	if (nullptr != m_equiped_item && item_object == m_equiped_item)
	{
		SlotCanvasPanel->ClearChildren();

		m_item_widget->InitItemWidget(ItemOnRemoved, m_equiped_item, m_tile_size);

		AC0000* player = Cast<AC0000>(GetOwningPlayerPawn());
		if (nullptr == player) { return drag_result; }

		if (EEquipmentType::EET_Weapon == m_slot_type) {
			player->UnequipWeapon(m_slot_type, !(FString("LeftWeaponSlot") == this->GetName()));
		}
		else {
			player->UnequipArmor(m_slot_type);
		}
	}

	return true;
}

void UInventoryEquipSlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	FLinearColor box_color;

	UItemObject* item_object = Cast<UItemObject>(InOperation->Payload);
	if (nullptr == item_object) { box_color = FLinearColor(1, 0, 0, 0.2); }

	bool is_matching = m_slot_type == item_object->GetItemEquipType();
	box_color = is_matching ? FLinearColor(0, 1, 0, 0.2) : FLinearColor(1, 0, 0, 0.2);

	SlotBorder->SetBrushColor(box_color);
}

void UInventoryEquipSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	SlotBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.2));
}

bool UInventoryEquipSlot::IsEnableEquipment(UItemObject* const item_object)
{
	EEquipmentType type = item_object->GetItemEquipType();

	if (m_slot_type != type)
	{
		if (EDragAndDropHUD::EDAD_Equipment == item_object->GetCurrentHUD())
		{
			if (nullptr == m_inventory_equip) { return false; }
			UInventoryEquipSlot* slot = m_inventory_equip->GetMatchingItemSlot(item_object->GetItemEquipType(), item_object);
			if (nullptr == slot) { return false; }

			slot->EquipItem(item_object);
			SlotBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.2));

			return false;
		}
		else
		{
			ItemOnAdd.Execute(item_object, item_object->GetLastInventoryLocation());

			SlotBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.2));

			return false;
		}
	}

	return true;
}
