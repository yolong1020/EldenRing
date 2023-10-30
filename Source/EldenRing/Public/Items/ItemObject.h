// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Equipment/EquipmentTypes.h"
#include "ItemData.h"
#include "../HUD/Structure/InventoryTile.h"
#include "../HUD/Inventory/InventoryTypes.h"
#include "ItemObject.generated.h"

class UMaterialInterface;
class AItem;

struct FInventoryTile;

UCLASS()
class BASIC_API UItemObject : public UObject
{
	GENERATED_BODY()
	
public:
	FIntPoint GetDimension();

	void SetItemProperties(FItemData* const item_data, TSubclassOf<AItem> item_class, bool is_rotated);

	FORCEINLINE const EEquipmentType&	GetItemEquipType()			{ return m_item_data->EquipmentType; }
	FORCEINLINE const EDragAndDropHUD&	GetCurrentHUD()				{ return m_current_hud; }
	FORCEINLINE const FInventoryTile&	GetLastInventoryLocation()		{ return m_last_inventory_location; }
	FORCEINLINE FItemData* const		GetItmeData()				{ return m_item_data; }
	FORCEINLINE UMaterialInterface*		GetIconImage(bool is_rotated)		{ return is_rotated ? m_icon_rotated : m_icon; }
	FORCEINLINE TSubclassOf<AItem>		GetItemClass()				{ return m_item_class; }
	FORCEINLINE bool			IsRotated()				{ return m_is_rotated; }

	FORCEINLINE void RotateItemObject()						{ m_is_rotated = !m_is_rotated; }
	FORCEINLINE void SetCurrentHUD(const EDragAndDropHUD& hud_type)			{ m_current_hud = hud_type; }
	FORCEINLINE void SetLastInventoryLocation(const FInventoryTile& start_tile) 	{ m_last_inventory_location = start_tile; }

private:
	FItemData*		m_item_data;
	UMaterialInterface*	m_icon;
	UMaterialInterface*	m_icon_rotated;
	TSubclassOf<AItem>	m_item_class;

	FIntPoint		m_slot_size;
	FInventoryTile		m_last_inventory_location;

	EDragAndDropHUD		m_current_hud;

	bool			m_is_rotated;
	
};
