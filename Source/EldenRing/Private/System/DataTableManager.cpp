// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DataTableManager.h"
#include "Macro/DebugMacros.h"

UDataTable* const UDataTableManager::GetDataTable(const EDataTableType& Type)
{
	if (m_data.Contains(Type)) return m_data[Type];
	else return nullptr;
}

void UDataTableManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UDataTable* item_table		= LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/Item/DT_ItemData.DT_ItemData'"), nullptr, LOAD_None, nullptr);
	UDataTable* itemprop_table	= LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/Item/DT_ItemProperties.DT_ItemProperties'"), nullptr, LOAD_None, nullptr);
	UDataTable* weapon_table	= LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/Item/DT_WeaponData.DT_WeaponData'"), nullptr, LOAD_None, nullptr);
	UDataTable* vendor_table	= LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/Item/DT_ItemVender.DT_ItemVender'"), nullptr, LOAD_None, nullptr);
	
	CHECK_INVALID_PTR(item_table)
	CHECK_INVALID_PTR(itemprop_table)
	CHECK_INVALID_PTR(weapon_table)
	CHECK_INVALID_PTR(vendor_table)

	m_data.Add(EDataTableType::EDTT_Item, item_table);
	m_data.Add(EDataTableType::EDTT_ItemProp, itemprop_table);
	m_data.Add(EDataTableType::EDTT_Weapon, weapon_table);
	m_data.Add(EDataTableType::EDTT_Vender, vendor_table);
}
