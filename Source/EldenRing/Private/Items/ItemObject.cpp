// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemObject.h"
#include "Items/ItemData.h"
#include "Macro/DebugMacros.h"

void UItemObject::BeginDestroy()
{
	Super::BeginDestroy();
}

void UItemObject::SetItemProperties(const FName& RowName, const FItemData& Data, TSubclassOf<AItem> Class, bool IsRotated)
{
	ItemRowName		= RowName;
	ItemData		= Data;
	ItemClass		= Class;
	IsRotatedItem	= IsRotated;
	ItemSlotSize	= ItemData.SlotSize;

	Icon			= LoadObject<UMaterialInterface>(nullptr, *Data.IconPath);
	Icon_Rotated	= LoadObject<UMaterialInterface>(nullptr, *Data.IconPathRotated);
}

void UItemObject::RotateItemObject()
{
	IsRotatedItem = !IsRotatedItem;
}

const FIntPoint UItemObject::GetDimension() const
{
	if (false == IsRotatedItem) return ItemSlotSize;

	return FIntPoint(ItemSlotSize.Y, ItemSlotSize.X);
}