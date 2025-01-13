// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ERSaveData.h"
#include "Items/ItemObject.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/NPC/Vender/Vender_Interface.h"
#include "FieldObject/PatrolPoint/PatrolPoint.h"
#include "Macro/DebugMacros.h"

void UERSaveData::SetEquipedItems(const TArray<TObjectPtr<UItemObject>>& Items)
{
	m_equiped_items.Empty();
	for (int32 i = (int32)EEquipmentType::EET_Head; i <= (int32)EEquipmentType::EET_Consumable; ++i)
	{
		if (!Items[i - 1]) continue;

		FEquipSaveData equip(Items[i - 1]->GetRowName(), i == (int32)EEquipmentType::EET_Weapon ? true : false);
		m_equiped_items.Add(equip);
	}
}

void UERSaveData::SetInventoryItems(const TArray<TObjectPtr<UItemObject>>& Items)
{
	m_inventory_items.Empty();

	TSet<TObjectPtr<UItemObject>> saved;
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (!Items[i] || saved.Contains(Items[i])) continue;

		FInventorySaveData data(Items[i]->GetRowName(), Items[i]->IsRotated(), Items[i]->GetLastInventoryLocation());
		m_inventory_items.Add(data);

		saved.Add(Items[i]);
	}
}

void UERSaveData::SetNpcObject(const TScriptInterface<INPC_Interfaces>& NPC_Interface)
{
	IGameCharacter_Interface* character_interface = Cast<IGameCharacter_Interface>(NPC_Interface.GetObject());
	CHECK_INVALID_PTR(character_interface)
	const bool is_boss = character_interface->IsHasTag(FName("Boss"));

	FGameObjectSaveData npc_data;
	npc_data.Transform			= character_interface->GetStartTransform();
	npc_data.ClassType			= character_interface->GetClassType();
	npc_data.WeaponClassTypes	= character_interface->GetWeaponClassTypes();
	npc_data.WeaponEquipState	= character_interface->GetWeaponEquipState();

	if		(is_boss)										 npc_data.ActionState = (uint8)EActionState_NPC::EASN_Unoccupied;
	else if (character_interface->IsHasTag(FName("Vender"))) npc_data.ActionState = NPC_Interface->GetActionState();
	else													 npc_data.ActionState = (uint8)EActionState_NPC::EASN_Patrolling;

	const TArray<TObjectPtr<APatrolPoint>> points = NPC_Interface->GetPatrolPoints();
	for (APatrolPoint* const point : points)
	{
		npc_data.PatrolPoints.Add(point->GetUniqueName());
	}

	npc_data.AssemblyPointLabel		= NPC_Interface->GetAssemblyPointLabel();
	npc_data.WaitTime_Min			= NPC_Interface->GetWaitTime_Min();
	npc_data.WaitTime_Max			= NPC_Interface->GetWaitTime_Max();
	npc_data.PatrolTime				= NPC_Interface->GetPatrolTime();
	npc_data.KnowMissingTargetTime	= NPC_Interface->GetKnowMissingTargetTime();

	IInteractor_Interface*	interactor_interface = Cast<IInteractor_Interface>(NPC_Interface.GetObject());
	IVender_Interface*		vender_interface	 = Cast<IVender_Interface>(NPC_Interface.GetObject());
	if (interactor_interface && vender_interface)
	{
		npc_data.OptionKey		  = interactor_interface->GetOptionKey();
		npc_data.CurrentDialouge  = vender_interface->GetDialouge();
		npc_data.CommunicateIndex = vender_interface->GetCommunicateIndex();
		npc_data.CameraNames	  = vender_interface->GetCameras();
		npc_data.MovePointName	  = vender_interface->GetCurrentPointName();

		TArray<TObjectPtr<UItemObject>> items = *vender_interface->GetTradeItem();
		TSet<TObjectPtr<UItemObject>> saved;

		for (int32 i = 0; i < items.Num(); ++i)
		{
			if (!items[i] || saved.Contains(items[i])) continue;

			FInventorySaveData data(items[i]->GetRowName(), items[i]->IsRotated(), items[i]->GetLastInventoryLocation());
			npc_data.TradeItems.Add(data);

			saved.Add(items[i]);
		}
	}

	m_npcs.Add(npc_data);
	m_npcs.Shrink();
}

void UERSaveData::SetFieldObject(const TScriptInterface<IFieldObject_Interface>& FieldObject_Interface)
{
	FGameFieldObjectSaveData object_data;
	object_data.Transform = FieldObject_Interface->GetFieldObjectTransform();
	object_data.ClassType = FieldObject_Interface->GetClassType();

	m_field_objects.Add(object_data);
	m_field_objects.Shrink();
}

void UERSaveData::SetInteractObject(const TScriptInterface<IInteractor_Interface>& Interactor_Interface)
{
	FGameInteractorObjectSaveData interactor_data;
	interactor_data.InteractorName  = Interactor_Interface->GetInteractorName();
	interactor_data.IsFirstInteract = Interactor_Interface->IsFirstInteract();

	m_interactor_objects.Add(interactor_data);
	m_interactor_objects.Shrink();
}

void UERSaveData::SetTutorialObject(const TScriptInterface<ITutorial_Interface>& Tutorial_Interface)
{
	FGameTutorialSaveData tutorial_data;
	tutorial_data.Transform		= Tutorial_Interface->GetObjectTransform();
	tutorial_data.ClassType		= Tutorial_Interface->GetClassType();
	tutorial_data.TutorialDatas = Tutorial_Interface->GetCallOuts();

	m_tutorial_objects.Add(tutorial_data);
	m_tutorial_objects.Shrink();
}
