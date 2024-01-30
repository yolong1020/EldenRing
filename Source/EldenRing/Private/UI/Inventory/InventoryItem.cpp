// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryItem.h"
#include "UI/Inventory/InventoryEquipSlot.h"
#include "UI/Structure/InventoryTile.h"
#include "Items/ItemObject.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInputLibrary.h"
#include "System/StorageManager.h"
#include "System/EquipmentManager.h"
#include "GameBase/ERGameState.h"
#include "Macro/DebugMacros.h"

void UInventoryItem::InitItemWidget(const TObjectPtr<UItemObject> ItemObject, const FVector2D& SpaceSize, const float& TileSize)
{
	CHECK_INVALID_PTR(ItemObject)

	m_item_object = ItemObject;
	m_tile_size	  = TileSize;

	ItemRefresh(SpaceSize);
}

void UInventoryItem::InitItemWidget(const TObjectPtr<UItemObject> ItemObject, const FVector2D& SpaceSize, const float& TileSize, const FVector2D& CustomSize)
{
	CHECK_INVALID_PTR(ItemObject)

	m_item_object = ItemObject;
	m_tile_size	  = TileSize;

	FVector2D scale	= FVector2D(CustomSize.X * m_tile_size, CustomSize.Y * m_tile_size);

	if (CustomSize.X > 1) { scale.X += (SpaceSize.X * CustomSize.X); }
	if (CustomSize.Y > 1) { scale.Y += (SpaceSize.Y * CustomSize.Y); }

	m_slot_size.Set(scale.X, scale.Y);
	Refresh();
}

void UInventoryItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	m_storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_equip_mgr = instance->GetSubsystem<UEquipmentManager>();
	CHECK_INVALID_SMART_PTR(m_equip_mgr)
}

int32 UInventoryItem::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 result = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	m_widget_size = AllottedGeometry.GetLocalSize();

	return result;
}

FReply UInventoryItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (EKeys::RightMouseButton == InMouseEvent.GetEffectingButton())
	{
		EDragAndDropHUD current = m_item_object->GetCurrentHUD();
		if (current == EDragAndDropHUD::EDAD_Equipment)
		{
			UInventoryEquipSlot* slot = m_equip_mgr->GetMatchingSlot(m_item_object.Get());
			CHECK_INVALID_PTR_RetVal(slot)

			m_storage_mgr->TryAddItem(m_item_object);
			m_equip_mgr->TryUnequipItem(m_item_object->GetItemEquipType(), slot->GetFName() == FName("RightWeaponSlot"));
			m_item_object->SetLastEquipSlotName(slot->GetName());
		}
		else if (current == EDragAndDropHUD::EDAD_Inventory)
		{
			UInventoryEquipSlot* slot = m_equip_mgr->FindEquipSlot(m_item_object);
			CHECK_INVALID_PTR_RetVal(slot)

			const EEquipmentType slot_type = slot->GetSlotType();
			const FString		 slot_name = slot->GetName();

			if (false == slot->IsEmpty())
			{
				const TObjectPtr<UItemObject> equiped_item = slot->GetEquipedItem();
				CHECK_INVALID_PTR_RetVal(equiped_item)

				m_equip_mgr->TryUnequipItem(slot_type, FString("RightWeaponSlot") == slot_name);
				m_storage_mgr->TryAddItem(equiped_item);
			}

			m_equip_mgr->TryEquipItem(m_item_object, slot_type, (FString("RightWeaponSlot") == slot_name));
			m_storage_mgr->TryRemoveItem(m_item_object);
		}

		m_storage_mgr->HideItemInfo();
		RemoveFromParent();
	}

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UInventoryItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	m_widget_drag = UWidgetBlueprintLibrary::CreateDragDropOperation(m_drag_drop_class);
	m_widget_drag->Payload			 = m_item_object.Get();
	m_widget_drag->DefaultDragVisual = this;
	m_widget_drag->Pivot			 = EDragPivot::CenterCenter;
	m_widget_drag->Offset			 = FVector2D(0, 0);
	OutOperation					 = m_widget_drag.Get();

	m_storage_mgr->TryRemoveItem(m_item_object.Get());
	m_storage_mgr->HideItemInfo();

	RemoveFromParent();
}

void UInventoryItem::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	if (InDragDropEvent.GetEffectingButton().GetFName().IsNone())
	{
		UItemObject* item_object = Cast<UItemObject>(InOperation->Payload);
		if (item_object)
		{
			switch (item_object->GetCurrentHUD())
			{
			case EDragAndDropHUD::EDAD_Inventory:
				m_storage_mgr->TryAddItemAtTile(item_object, item_object->GetLastInventoryLocation());
				break;
			case EDragAndDropHUD::EDAD_Equipment:
				m_equip_mgr->TryEquipItem(item_object, item_object->GetItemEquipType(), item_object->GetLastEquipSlotName() == FString("RightWeaponSlot"));
				break;
			}
		}
	}
}

void UInventoryItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	FVector2D screen	= FVector2D::ZeroVector;
	FVector2D viewport	= FVector2D::ZeroVector;
	USlateBlueprintLibrary::LocalToViewport(GetOwningPlayer(), InGeometry, FVector2D::ZeroVector, screen, viewport);

	CHECK_INVALID_PTR(m_item_object)
	m_storage_mgr->ShowItemInfo(viewport, m_widget_size, m_item_object);

	CHECK_INVALID_PTR(BGBorder);
	BGBorder->SetBrushColor(FLinearColor(0, 1, 0, 0.25));

	SetFocus();
}

void UInventoryItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	CHECK_INVALID_PTR(BGBorder);
	BGBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.25));

	m_storage_mgr->HideItemInfo();
}

FReply UInventoryItem::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (EKeys::C == InKeyEvent.GetKey())
	{
		EDragAndDropHUD current = m_item_object->GetCurrentHUD();
		if (current == EDragAndDropHUD::EDAD_Equipment)
		{
			UInventoryEquipSlot* slot = m_equip_mgr->GetMatchingSlot(m_item_object.Get());
			CHECK_INVALID_PTR_RetVal(slot)

			m_storage_mgr->HideItemInfo();
			m_equip_mgr->TryUnequipItem(m_item_object->GetItemEquipType(), slot->GetFName() == FName("RightWeaponSlot"));
			m_item_object->SetLastEquipSlotName(slot->GetName());

			AERGameState* game_state = Cast<AERGameState>(UGameplayStatics::GetGameState(this));
			game_state->SpawnItemFromObject(GetOwningPlayerPawn(), m_item_object.Get(), true);

			RemoveFromParent();
		}
		else if (current == EDragAndDropHUD::EDAD_Inventory)
		{
			m_storage_mgr->HideItemInfo();
			m_storage_mgr->TryRemoveItem(m_item_object);

			AERGameState* game_state = Cast<AERGameState>(UGameplayStatics::GetGameState(this));
			game_state->SpawnItemFromObject(GetOwningPlayerPawn(), m_item_object.Get(), true);

			RemoveFromParent();
		}
	}

	return result;
}

void UInventoryItem::Refresh()
{
	CHECK_INVALID_PTR(m_item_object)

	BGSizeBox->SetWidthOverride(m_slot_size.X);
	BGSizeBox->SetHeightOverride(m_slot_size.Y);

	bool is_rotated = m_item_object->IsRotated();
	TObjectPtr<UMaterialInterface> icon = m_item_object->GetIconImage(is_rotated);

	ItemImage->SetBrushFromMaterial(icon.Get());
	ItemImage->SetDesiredSizeOverride(FVector2D(1, 1));
}

void UInventoryItem::ItemRefresh(const FVector2D& SpaceSize)
{
	CHECK_INVALID_PTR(m_item_object)

	FIntPoint dimension = m_item_object->GetDimension();
	FVector2D scale		= FVector2D(dimension.X * m_tile_size, dimension.Y * m_tile_size);

	if (dimension.X > 1) { scale.X += (SpaceSize.X * (dimension.X - 1)); }
	if (dimension.Y > 1) { scale.Y += (SpaceSize.Y * (dimension.Y - 1)); }

	m_slot_size.Set(scale.X, scale.Y);
	Refresh();
}
