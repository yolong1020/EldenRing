// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "Items/Equipment/EquipmentTypes.h"
#include "UI/UITypes.h"
#include "InventoryEquipSlot.generated.h"

class AC0000;
class UStorageManager;
class UEquipmentManager;
class UInventoryEquip;
class UInventoryItem;
const UItemObject;
class UBorder;
class UImage;
class USizeBox;
class UWrapBox;

UCLASS()
class ELDENRING_API UInventoryEquipSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitSlotWidget(const EEquipmentType& equip_type, const float& tile_size, const FVector2D& slot_size);
	void EquipItem(const TObjectPtr<UItemObject> ItemObject);
	void UnequipItem();
	
	FORCEINLINE const EEquipmentType& GetSlotType() const					{ return m_slot_type; }
	FORCEINLINE bool IsEmpty()												{ return m_equiped_item == nullptr; }
	FORCEINLINE bool IsMatching(const TObjectPtr<UItemObject> item_object)	{ return m_equiped_item == item_object; }
	FORCEINLINE const TObjectPtr<UItemObject> GetEquipedItem()				{ return m_equiped_item != nullptr ? m_equiped_item : nullptr; }

protected:
	virtual void NativeOnInitialized() override final;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	bool IsEnableEquipment(const TObjectPtr<UItemObject> item_object);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItem> m_widget_class;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBorder> SlotBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> SlotBGImage;

	UPROPERTY()
	TObjectPtr<UInventoryItem> m_item_widget;

	UPROPERTY()
	TObjectPtr<UItemObject> m_equiped_item;

	UPROPERTY()
	TWeakObjectPtr<UStorageManager>	m_storage_mgr;

	UPROPERTY()
	TWeakObjectPtr<UEquipmentManager>  m_equipment_mgr;

	float				m_tile_size;
	FVector2D			m_slot_size;
	EEquipmentType		m_slot_type;
};
