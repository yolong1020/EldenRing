// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EquipmentManager.h"
#include "System/DataTableManager.h"
#include "HUD/Status/QuickSlotTypes.h"
#include "HUD/GameHUD_Actor.h"
#include "HUD/Status/GameHUDComponent.h"
#include "Items/ItemObject.h"
#include "Items/Item_Interface.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Items/Weapons/Shield_Actor.h"
#include "Characters/Component/AttributeComponent.h"
#include "Characters/Players/C0000.h"
#include "UI/Inventory/InventoryEquipSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMesh.h"
#include "Macro/DebugMacros.h"

void UEquipmentManager::SetEquipmentData(AC0000* const Player, const TArray<UInventoryEquipSlot*>&& Slots)
{
	m_player = Player;
	m_slots  = Slots;
}

void UEquipmentManager::InitEquipCallback(const FTryEquipItem& Callback)
{
	m_equip_delegate = Callback;
}

void UEquipmentManager::UnequipAllSlot()
{
	for (UInventoryEquipSlot* const slot : m_slots) { slot->UnequipItem(); }
}

bool UEquipmentManager::TryEquipItem(const TObjectPtr<UItemObject> ItemObject, const EEquipmentType& Type, const bool IsRight)
{
	switch (Type)
	{
		case EEquipmentType::EET_None:
		case EEquipmentType::EET_Max:
		case EEquipmentType::EET_Consumable: return false;
		default:
		{
			UInventoryEquipSlot* slot = nullptr;
			if (Type == EEquipmentType::EET_Weapon) { slot = IsRight ? m_slots[(int32)Type - 1] : m_slots[(int32)Type]; }
			else					{ slot = m_slots[(int32)Type - 1]; }

			if (slot == nullptr || !slot->IsEmpty()) return false;

			slot->EquipItem(ItemObject);
			UpdateMesh(ItemObject, Type, IsRight);
		}
	}

	m_player->AdjustItemOption(true, ItemObject);

	return true;
}

bool UEquipmentManager::TryUnequipItem(const EEquipmentType& Type, const bool IsRight)
{
	if (Type == EEquipmentType::EET_None		||
		Type == EEquipmentType::EET_Max			||
		Type == EEquipmentType::EET_Consumable) return false;

	TObjectPtr<UItemObject> item;
	if (Type == EEquipmentType::EET_Weapon)
	{
		m_player->ReleaseWeapon(IsRight);

		int32 slot_idx = IsRight ? int32(Type) - 1 : int32(Type);

		item = m_slots[slot_idx]->GetEquipedItem(); 
		m_slots[slot_idx]->UnequipItem();
	}
	else
	{
		if (nullptr == m_equipments[int32(Type) - 1].Key) return false;

		m_equipments[int32(Type) - 1].Key->SetSkeletalMesh(nullptr);
		m_equipments[int32(Type) - 1].Key->UnregisterComponent();
		m_equipments[int32(Type) - 1].Key = nullptr;

		MeshActivate(m_equipments[int32(Type) - 1].Value, true);

		item = m_slots[int32(Type) - 1]->GetEquipedItem();
		m_slots[int32(Type) - 1]->UnequipItem();
	}
	m_player->AdjustItemOption(false, item);

	return true;
}

UInventoryEquipSlot* const UEquipmentManager::GetMatchingSlot(const TObjectPtr<UItemObject> ItemObject)
{
	if (!ItemObject) return nullptr;

	EEquipmentType Type = ItemObject->GetItemEquipType();
	if (Type == EEquipmentType::EET_Weapon) 
	{
		bool is_right = m_slots[(int32)Type - 1]->IsMatching(ItemObject);
		bool is_left  = m_slots[(int32)Type]->IsMatching(ItemObject);

		if (is_right)		{ return m_slots[(int32)Type - 1]; }
		else if (is_left)	{ return m_slots[(int32)Type]; }
	}
	else
	{
		if (m_slots[(int32)Type - 1]->IsMatching(ItemObject)) return m_slots[(int32)Type - 1];
	}

	return nullptr;
}

UInventoryEquipSlot* const UEquipmentManager::FindEquipSlot(const TObjectPtr<UItemObject> ItemObject)
{
	if (!ItemObject) return nullptr;

	EEquipmentType Type = ItemObject->GetItemEquipType();
	if (Type == EEquipmentType::EET_Weapon)
	{
		AShield_Actor* is_shield = Cast<AShield_Actor>(ItemObject->GetItemClass().GetDefaultObject());

		if (is_shield)  { return m_slots[(int32)Type]; }
		else			{ return m_slots[(int32)Type - 1]; }
	}
	else { return m_slots[(int32)Type - 1]; }

	return nullptr;
}

const TArray<TObjectPtr<UItemObject>> UEquipmentManager::GetAllEquipedItems()
{
	TArray<TObjectPtr<UItemObject>> items;
	items.Reserve(m_slots.Num());

	for (UInventoryEquipSlot* const slot : m_slots)
	{
		items.Add(slot->GetEquipedItem());
	}

	return items;
}

TArray<TArray<UMaterialInstanceDynamic*>> UEquipmentManager::GetAllArmorMaterials()
{
	TArray<TArray<UMaterialInstanceDynamic*>> output;

	USkeletalMeshComponent* mesh = m_player->GetMesh();
	for (const TTuple<USkeletalMeshComponent*, TObjectPtr<UItemObject>> item : m_equipments)
	{
		if (!item.Key) continue;

		TArray<UMaterialInstanceDynamic*> item_materials;
		TArray<UMaterialInterface*> materials = item.Key->GetMaterials();
		for (int i = 0; i < materials.Num(); ++i)
		{
			if (!materials[i]) continue;

			UMaterialInstanceDynamic* dynamic = UMaterialInstanceDynamic::Create(materials[i], this);
			item.Key->SetMaterial(i, dynamic);
			item_materials.Add(dynamic);
		}
		item_materials.Shrink();
		
		output.Add(item_materials);
	}

	return output;
}

void UEquipmentManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_equipments.SetNum((int32)EEquipmentType::EET_Legs);
}

void UEquipmentManager::UpdateMesh(const TObjectPtr<UItemObject> ItemObject, const EEquipmentType& Type, const bool IsRight)
{
	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(m_player)

	USkeletalMeshComponent* mesh = m_player->GetMesh();
	CHECK_INVALID_PTR(mesh)

	if (EEquipmentType::EET_Weapon == Type)
	{
		AWeapon_Actor* weapon = world->SpawnActor<AWeapon_Actor>(ItemObject->GetItemClass());
		CHECK_INVALID_PTR(weapon)

		FString	    socket_name   = IsRight ? "R" : "L";
		EWeaponType weapon_type   = weapon->GetWeaponType();

		socket_name += ((EWeaponType::EWT_Shield == weapon_type) ? FString("_ShieldSocket") : FString("_WeaponSocket"));
		weapon->Equip(mesh, FName(socket_name), m_player, m_player);
		m_player->SetWeapon(IsRight, weapon);

		IPlayer_Interface* player_interface = Cast<IPlayer_Interface>(m_player);
		CHECK_INVALID_PTR(player_interface)
		UGameHUDComponent* hud = player_interface->GetHUDComponent();
		CHECK_INVALID_PTR(hud)
		hud->SetQuickSlotItem(IsRight ? EQuickSlotType::EQST_Right : EQuickSlotType::EQST_Left, weapon->GetItemObject());
	}
	else
	{
		USkeletalMeshComponent* new_mesh_component = NewObject<USkeletalMeshComponent>(m_player, UEnum::GetValueAsName(Type));
		new_mesh_component->SetupAttachment(mesh);

		const FItemData* item_data = ItemObject->GetItemData();
		USkeletalMesh* new_skeletal_mesh = LoadObject<USkeletalMesh>(nullptr, *item_data->MeshPath);
		CHECK_INVALID_PTR(new_skeletal_mesh)

		new_mesh_component->SetSkeletalMesh(new_skeletal_mesh);
		new_mesh_component->SetLeaderPoseComponent(mesh);
		new_mesh_component->RegisterComponent();

		m_equipments[int32(Type) - 1].Key   = new_mesh_component;
		m_equipments[int32(Type) - 1].Value = ItemObject;
		
		MeshActivate(ItemObject, false);
	}
}

void UEquipmentManager::MeshActivate(const TObjectPtr<UItemObject> ItemObject, const bool& IsShow)
{
	const FItemData* item_data = ItemObject->GetItemData();
	CHECK_INVALID_PTR(item_data)

	USkeletalMeshComponent* mesh = m_player->GetMesh();
	CHECK_INVALID_PTR(mesh)

	USkinnedMeshComponent* SkinnedMeshComp = m_player->FindComponentByClass<USkinnedMeshComponent>();
	CHECK_INVALID_PTR(SkinnedMeshComp)

	const TArray<int32>& hide_materials = item_data->HideMaterials;
	for (const int32& material_index : hide_materials)
	{
		SkinnedMeshComp->ShowMaterialSection(material_index, 0, IsShow, 0);
	}
}
