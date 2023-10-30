// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemObject.h"
#include "Items/ItemData.h"
#include "Basic/DebugMacros.h"

void UItemObject::SetItemProperties(FItemData* const item_data, TSubclassOf<AItem> item_class, bool is_rotated)
{
	CHECK_INVALID(item_data)

	m_item_data		= item_data;
	m_item_class	= item_class;
	m_is_rotated	= is_rotated;
	m_slot_size		= m_item_data->SlotSize;

	m_icon			= LoadObject<UMaterialInterface>(nullptr, *item_data->IconPath);
	m_icon_rotated	= LoadObject<UMaterialInterface>(nullptr, *item_data->IconPathRotated);
}

FIntPoint UItemObject::GetDimension()
{
	if (false == m_is_rotated)  { return m_slot_size; }

	return FIntPoint(m_slot_size.Y, m_slot_size.X);
}