// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../HUD_Delegates.h"
#include "../../Items/Equipment/EquipmentTypes.h"
#include "InventoryTypes.h"
#include "InventoryEquipSlot.generated.h"

class UInventoryEquip;
class UInventoryItem;
class UInventoryComponent;
class UBorder;
class UImage;
class UCanvasPanel;

UCLASS()
class BASIC_API UInventoryEquipSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitSlotWidget(UInventoryEquip* const widget_equip, FItemOnAdd& func_add, FItemOnAddAtEmpty& func_add_empty, FItemOnRemoved& func_remove, const EEquipmentType& equip_type, const float& tile_size, const FVector2D& slot_size);
	void EquipItem(UItemObject* const item_object);
	void UnequipItem();

	FORCEINLINE bool IsEmpty()									{ return m_equiped_item == nullptr; }
	FORCEINLINE bool IsMatching(UItemObject* const item_object) { return m_equiped_item == item_object; }

protected:
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool	NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	bool IsEnableEquipment(UItemObject* const item_object);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItem> m_widget_class;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	UBorder* SlotBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	UCanvasPanel* SlotCanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	UImage* SlotBGImage;

	UPROPERTY()
	UInventoryEquip*	m_inventory_equip;

	UPROPERTY()
	UInventoryItem*		m_item_widget;

	UPROPERTY()
	UItemObject*		m_equiped_item;

	float				m_tile_size;
	FVector2D			m_slot_size;
	EEquipmentType		m_slot_type;

	FItemOnAdd			ItemOnAdd;
	FItemOnAddAtEmpty	ItemOnAddEmpty;
	FItemOnRemoved		ItemOnRemoved;
};
