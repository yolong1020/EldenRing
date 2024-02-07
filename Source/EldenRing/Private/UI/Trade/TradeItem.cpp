// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trade/TradeItem.h"
#include "UI/Structure/InventoryTile.h"
#include "UI/Inventory/InventoryEquipSlot.h"
#include "UI/UIDelegates.h"
#include "UI/PopUp/PopUp.h"
#include "UI/PopUp/PopUpTypes.h"
#include "UI/Button/ButtonType.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/RichTextBlock.h"
#include "System/StorageManager.h"
#include "System/TradeManager.h"
#include "System/EquipmentManager.h"
#include "System/ObserverManager.h"
#include "GameBase/ERGameInstance.h"
#include "Items/ItemObject.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Players/C0000.h"
#include "Characters/NPC/Vender/Vender.h"
#include "Macro/DebugMacros.h"

void UTradeItem::InitItemWidget(const bool IsPlayer, const TObjectPtr<UItemObject> item_object, const float& tile_size)
{
	m_item_object	= item_object;
	m_tile_size	= tile_size;
	m_is_player	= IsPlayer;

	ItemRefresh();
}

void UTradeItem::InitItemWidget(const bool IsPlayer, const TObjectPtr<UItemObject> item_object, const float& tile_size, const FVector2D& custom_size)
{
	m_item_object	= item_object;
	m_tile_size	= tile_size;
	m_is_player	= IsPlayer;

	m_slot_size.Set(custom_size.X * m_tile_size, custom_size.Y * m_tile_size);
	EquipedItemRefresh();
}

void UTradeItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	m_trade_mgr = instance->GetSubsystem<UTradeManager>();
	CHECK_INVALID_SMART_PTR(m_trade_mgr)
	m_storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_equip_mgr = instance->GetSubsystem<UEquipmentManager>();
	CHECK_INVALID_SMART_PTR(m_equip_mgr)
}

int32 UTradeItem::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 result = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	m_widget_size = AllottedGeometry.GetLocalSize();

	return result;
}

void UTradeItem::ItemRefresh()
{
	FIntPoint dimension = m_item_object->GetDimension();

	m_slot_size.Set(dimension.X * m_tile_size, dimension.Y * m_tile_size);

	Refresh();
}

void UTradeItem::EquipedItemRefresh()
{
	Refresh();
}

FReply UTradeItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply	result	  = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	FKey	event_key = InMouseEvent.GetEffectingButton();

	if (EKeys::RightMouseButton == event_key)
	{
		EDragAndDropHUD current = m_item_object->GetCurrentHUD();
		if	(current == EDragAndDropHUD::EDAD_Vender)    { TradeProcess(true); }
		else if (current == EDragAndDropHUD::EDAD_Inventory) { TradeProcess(false); }

		m_trade_mgr->HideItemInfo();
	}
	else if (EKeys::LeftMouseButton == event_key)
	{
		if (EDragAndDropHUD::EDAD_Vender == m_item_object->GetCurrentHUD())
		{
			UPopUp* popup = m_trade_mgr->GetPopup();
			if (popup == nullptr) return result;

			AC0000*	player = GetOwningPlayerPawn<AC0000>();
			int32	gold   = player->GetTotalGold();
			const FItemData* data = m_item_object->GetItemData();
			CHECK_INVALID_PTR_RetVal(data)

			if (gold >= data->Cost)
			{
				FPopupActiveBtn Active;
				Active.BindLambda([&]()->void { TradeProcess(true); });
				popup->UpdataTwoButtonForTrade(EPopupType::EPT_Purchase, m_item_object, Active, FPopupCancelBtn());
			}
			else 
			{ 
				popup->UpdateOneButtonLayout(EPopupType::EPT_NotEnoughGold, FPopupConfirmlBtn());
			}
		}

		m_trade_mgr->HideItemInfo();
	}

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UTradeItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (EDragAndDropHUD::EDAD_Vender != m_item_object->GetCurrentHUD())
	{
		CreateDragDrop(OutOperation);
		m_trade_mgr->TryRemoveItem(m_is_player, m_item_object);
		m_trade_mgr->HideItemInfo();
		RemoveFromParent();
	}
}

void UTradeItem::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	if (InDragDropEvent.GetEffectingButton().GetFName().IsNone())
	{
		UItemObject* item_object = Cast<UItemObject>(InOperation->Payload);
		if (item_object && item_object->GetCurrentHUD() == EDragAndDropHUD::EDAD_Inventory)
		{
			m_trade_mgr->TryAddItemAtTile(true, item_object, item_object->GetLastInventoryLocation());
		}
	}
}

void UTradeItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	FVector2D screen   = FVector2D::ZeroVector;
	FVector2D viewport = FVector2D::ZeroVector;
	USlateBlueprintLibrary::LocalToViewport(GetOwningPlayer(), InGeometry, FVector2D::ZeroVector, screen, viewport);

	m_trade_mgr->ShowItemInfo(viewport, m_widget_size, m_item_object);

	CHECK_INVALID_PTR(BGBorder);
	BGBorder->SetBrushColor(FLinearColor(0, 1, 0, 0.25));

	SetFocus();
}

void UTradeItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	m_trade_mgr->HideItemInfo();

	CHECK_INVALID_PTR(BGBorder);
	BGBorder->SetBrushColor(FLinearColor(0, 0, 0, 0.25));
}

FReply UTradeItem::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (EKeys::C == InKeyEvent.GetKey())
	{
		EDragAndDropHUD current = m_item_object->GetCurrentHUD();
		if (current == EDragAndDropHUD::EDAD_Vender)
		{
			UInventoryEquipSlot* slot = m_equip_mgr->FindEquipSlot(m_item_object.Get());
			CHECK_INVALID_PTR_RetVal(slot)

			const EEquipmentType slot_type = slot->GetSlotType();
			const FString	     slot_name = slot->GetName();

			if (false == slot->IsEmpty())
			{
				const TObjectPtr<UItemObject> equiped_item = slot->GetEquipedItem();
				CHECK_INVALID_PTR_RetVal(equiped_item)

				m_trade_mgr->TryAddItem(true, equiped_item);
				m_equip_mgr->TryUnequipItem(slot_type, FString("RightWeaponSlot") == slot_name);
			}

			m_equip_mgr->TryEquipItem(m_item_object, slot_type, FName("RightWeaponSlot") == slot_name);
			m_item_object->SetLastEquipSlotName(slot_name);

			TradeProcess(true, true);
		}
		else if (current == EDragAndDropHUD::EDAD_Inventory)
		{
			UInventoryEquipSlot* slot = m_equip_mgr->FindEquipSlot(m_item_object.Get());
			CHECK_INVALID_PTR_RetVal(slot)

			const EEquipmentType slot_type = slot->GetSlotType();
			const FString	     slot_name = slot->GetName();

			if (false == slot->IsEmpty())
			{
				const TObjectPtr<UItemObject> equiped_item = slot->GetEquipedItem();
				CHECK_INVALID_PTR_RetVal(equiped_item)

				m_trade_mgr->TryAddItem(true, equiped_item);
				m_equip_mgr->TryUnequipItem(slot_type, FString("RightWeaponSlot") == slot_name);
			}

			m_trade_mgr->TryRemoveItem(true, m_item_object);
			m_equip_mgr->TryEquipItem(m_item_object, slot_type, FName("RightWeaponSlot") == slot_name);
			m_item_object->SetLastEquipSlotName(slot_name);

			UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
			CHECK_INVALID_PTR_RetVal(instance)
			AC0000* player = GetOwningPlayerPawn<AC0000>();
			instance->SavePlayer(player);
		}

		m_trade_mgr->HideItemInfo();
		RemoveFromParent();
	}

	return result;
}

void UTradeItem::Refresh()
{
	CHECK_INVALID_PTR(m_item_object)

	BGSizeBox->SetWidthOverride(m_slot_size.X);
	BGSizeBox->SetHeightOverride(m_slot_size.Y);

	bool is_rotated = m_item_object->IsRotated();
	TWeakObjectPtr<UMaterialInterface> icon = m_item_object->GetIconImage(is_rotated);

	ItemImage->SetBrushFromMaterial(icon.Get());
	ItemImage->SetDesiredSizeOverride(FVector2D(1, 1));
}

void UTradeItem::CreateDragDrop(UDragDropOperation*& OutOperation)
{
	m_widget_drag = UWidgetBlueprintLibrary::CreateDragDropOperation(m_drag_drop_class);
	m_widget_drag->Payload = m_item_object;
	m_widget_drag->DefaultDragVisual = this;
	m_widget_drag->Pivot = EDragPivot::CenterCenter;
	m_widget_drag->Offset = FVector2D(0, 0);

	OutOperation = m_widget_drag;
}

void UTradeItem::TradeProcess(const bool IsPurchase, const bool IsPurchaseEquip)
{
	const FItemData* data = m_item_object->GetItemData();
	CHECK_INVALID_PTR_RetVal(data)

	AC0000*		player = GetOwningPlayerPawn<AC0000>();
	AGameCharacter* vender = Cast<AGameCharacter>(m_trade_mgr->GetVender());

	if (nullptr != data && player && vender)
	{
		if (IsPurchase)
		{
			player->SetGold(-data->Cost);
			vender->SetGold(data->Cost);
		}
		else
		{
			TArray<TObjectPtr<UItemObject>>* items = Cast<AVender>(vender)->GetTradeItem();
			items->Add(m_item_object);

			player->SetGold(data->Cost * 0.5);
			vender->SetGold(-(data->Cost * 0.5));
		}

		m_trade_mgr->TryUpdateGold(true, player->GetTotalGold());
		m_trade_mgr->TryUpdateGold(false, vender->GetTotalGold());

		if (IsPurchase && IsPurchaseEquip)
		{
			m_trade_mgr->TryRemoveItem(!IsPurchase, m_item_object);
		}
		else
		{
			m_trade_mgr->TryAddItem(IsPurchase, m_item_object);
			m_trade_mgr->TryRemoveItem(!IsPurchase, m_item_object);
		}

		RemoveFromParent();

		UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
		CHECK_INVALID_PTR_RetVal(instance)
		instance->SavePlayer(player);
		instance->ResetSaveData();
		UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
		CHECK_INVALID_PTR_RetVal(observer_mgr)
		observer_mgr->TriggerEvent(EObserverEventType::EOET_SaveData);
	}
}
