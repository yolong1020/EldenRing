// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/ERGameInstance.h"
#include "System/EquipmentManager.h"
#include "System/StorageManager.h"
#include "System/TradeManager.h"
#include "System/DataTableManager.h"
#include "System/UIManager.h"
#include "System/SoundscapeManager.h"
#include "Levels/BaseLevelScript.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/ERSaveData.h"
#include "Items/ItemObject.h"
#include "FieldObject/FieldObject.h"
#include "Characters/NPC/Vender/Vender_Interface.h"
#include "Macro/DebugMacros.h"

void UERGameInstance::RequestLevelLoadCompleteEvent()
{
	m_event.Broadcast();
	m_event.Clear();
}

void UERGameInstance::OpenLevel(const FString& LevelPath, const FName& LevelName)
{
	USoundscapeManager* sound_mgr = GetSubsystem<USoundscapeManager>();
	CHECK_INVALID_PTR(sound_mgr)
	sound_mgr->StopStageBGM();

	m_load_state = ELevelLoadState::ELLS_Loading;

	LoadPackageAsync(LevelPath,
	FLoadPackageAsyncDelegate::CreateLambda([&, LevelPath, LevelName](const FName& Name, UPackage* LoadPackage, EAsyncLoadingResult::Type Result)->void {
			if (Result == EAsyncLoadingResult::Succeeded) {
				UGameplayStatics::OpenLevel(this, LevelName);
				UE_LOG(LogTemp, Warning, TEXT("[Open Level]"))
			}
	}), 0, PKG_ContainsMap);

	m_load_state = ELevelLoadState::ELLS_Start;
}

void UERGameInstance::LoadLevelObjects()
{
	LoadNPC();
	LoadFieldObject();
	LoadInteractObject();
	LoadTutorialObject();
}

void UERGameInstance::SavePlayer(const TScriptInterface<IGameCharacter_Interface>& Character_Interface)
{
	CHECK_INVALID_PTR(Character_Interface.GetObject())

	TWeakObjectPtr<UERSaveData> game_data = GetGameData();
	CHECK_INVALID_SMART_PTR(game_data)

	UEquipmentManager* equip_mgr = GetSubsystem<UEquipmentManager>();
	CHECK_INVALID_PTR(equip_mgr)
	UStorageManager* storage_mgr = GetSubsystem<UStorageManager>();
	CHECK_INVALID_PTR(storage_mgr)

	game_data->SetEquipedItems(equip_mgr->GetAllEquipedItems());
	game_data->SetInventoryItems(*storage_mgr->GetSlots());
	game_data->SetCharacterGold(Character_Interface->GetTotalGold());
	game_data->SetStartPoint(Character_Interface->GetStartTransform());
	
	if (false == UGameplayStatics::SaveGameToSlot(game_data.Get(), FString("SavedGame"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Error"));
	}
}

void UERGameInstance::LoadPlayer(const TScriptInterface<IPlayer_Interface>& Player_Interface)
{
	CHECK_INVALID_PTR(Player_Interface.GetObject())

	if (UGameplayStatics::DoesSaveGameExist(FString("SavedGame"), 0))
	{
		UERSaveData* loaded_data = Cast<UERSaveData>(UGameplayStatics::LoadGameFromSlot(FString("SavedGame"), 0));
		CHECK_INVALID_PTR(loaded_data)

		IGameCharacter_Interface* character_interface = Cast<IGameCharacter_Interface>(Player_Interface.GetObject());
		CHECK_INVALID_PTR(character_interface)
		character_interface->SetCharacterTransform(loaded_data->GetStartPoint());

		TArray<FInventorySaveData>	stored  = loaded_data->GetInventoryItems();
		TArray<FEquipSaveData>		equiped = loaded_data->GetEquipedItems();

		UDataTableManager* datatable_mgr = GetSubsystem<UDataTableManager>();
		CHECK_INVALID_PTR(datatable_mgr)
		UDataTable* item_table = datatable_mgr->GetDataTable(EDataTableType::EDTT_Item);
		CHECK_INVALID_PTR(item_table)

		if (!stored.IsEmpty())
		{
			for (const FInventorySaveData& Item : stored)
			{
				FItemData* item_data = item_table->FindRow<FItemData>(Item.RowName, Item.RowName.ToString());
				CHECK_INVALID_PTR(item_data)

				UClass* item_class = LoadClass<AItem>(nullptr, *item_data->BluePrintPath);
				CHECK_INVALID_PTR(item_class)

				TObjectPtr<UItemObject> item_object = NewObject<UItemObject>();
				item_object->SetItemProperties(Item.RowName, *item_data, item_class, Item.IsRotated);

				UStorageManager* storage_mgr = GetSubsystem<UStorageManager>();
				CHECK_INVALID_PTR(storage_mgr)
				storage_mgr->TryAddItemAtTile(item_object, Item.Location);
			}
		}
		if (!equiped.IsEmpty())
		{
			for (const FEquipSaveData& Item : equiped)
			{
				FItemData* item_data = item_table->FindRow<FItemData>(Item.RowName, Item.RowName.ToString());
				CHECK_INVALID_PTR(item_data)

				UClass* item_class = LoadClass<AItem>(nullptr, *item_data->BluePrintPath);
				CHECK_INVALID_PTR(item_class)

				TObjectPtr<UItemObject> item_object = NewObject<UItemObject>();
				item_object->SetItemProperties(Item.RowName, *item_data, item_class, false);

				UEquipmentManager* equipment_mgr = GetSubsystem<UEquipmentManager>();
				CHECK_INVALID_PTR(equipment_mgr)
				equipment_mgr->TryEquipItem(item_object, item_object->GetItemEquipType(), Item.IsRight);
			}
		}
	}
}

void UERGameInstance::SaveNPC(const TScriptInterface<INPC_Interfaces>& NPC_Interface)
{
	CHECK_INVALID_PTR(NPC_Interface.GetObject())

	TWeakObjectPtr<UERSaveData> game_data = GetGameData();
	CHECK_INVALID_SMART_PTR(game_data)
	game_data->SetNpcObject(NPC_Interface);

	if (false == UGameplayStatics::SaveGameToSlot(game_data.Get(), FString("SavedGame"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Error"));
	}
}

void UERGameInstance::LoadNPC()
{
	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	TArray<AActor*> monsters;
	UGameplayStatics::GetAllActorsWithTag(this, FName("NPC"), monsters);
	for (AActor* const monster : monsters)
	{ 
		monster->LifeSpanExpired();
	}

	if (UGameplayStatics::DoesSaveGameExist(FString("SavedGame"), 0))
	{
		UERSaveData* loaded_data = Cast<UERSaveData>(UGameplayStatics::LoadGameFromSlot(FString("SavedGame"), 0));
		CHECK_INVALID_PTR(loaded_data)

		TArray<FGameObjectSaveData> datas = loaded_data->GetNPCs();
		for (const FGameObjectSaveData& data : datas)
		{
			FActorSpawnParameters param;
			param.bNoFail = true;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AGameCharacter* character = world->SpawnActor<AGameCharacter>(data.ClassType, data.Transform, param);
			CHECK_INVALID_PTR(character)
			bool is_vender = character->ActorHasTag(FName("Vender"));

#if WITH_EDITOR
			character->SetFolderPath(is_vender ? FName("Vender") : FName("Enemy"));
#endif
			IGameCharacter_Interface*	character_interface = Cast<IGameCharacter_Interface>(character);
			INPC_Interfaces*			npc_interface		= Cast<INPC_Interfaces>(character);
			CHECK_INVALID_PTR(character_interface)
			CHECK_INVALID_PTR(npc_interface)

			character_interface->SetCharacterTransform(data.Transform);
			character_interface->SetWeaponEquipState(StaticEnum<EEquipState>()->IsValidEnumValue(data.WeaponEquipState) ? (EEquipState)data.WeaponEquipState : EEquipState::EES_Unequipped);
			npc_interface->SetNPCActionState(StaticEnum<EActionState_NPC>()->IsValidEnumValue(data.ActionState) ? (EActionState_NPC)data.ActionState : EActionState_NPC::EASN_Unoccupied);
			npc_interface->SetWeaponClassTypes(data.WeaponClassTypes);
			npc_interface->SetTimeValues(data.WaitTime_Min, data.WaitTime_Max, data.PatrolTime, data.KnowMissingTargetTime);
			npc_interface->SetPatrolPoints(data.PatrolPoints);
			npc_interface->SetAssemblyPoint(data.AssemblyPointLabel);

			IVender_Interface* vender_interface = Cast<IVender_Interface>(character);
			IInteractor_Interface* interactor_interface = Cast<IInteractor_Interface>(character);
			if (vender_interface && interactor_interface)
			{
				interactor_interface->SetOptionKey(data.OptionKey);
				vender_interface->SetTradeItems(data.TradeItems);
				vender_interface->SetDialouge(data.CurrentDialouge);
				vender_interface->SetCommunicateIndex(data.CommunicateIndex);
				vender_interface->SetCameras(data.CameraNames);
				vender_interface->SetCurrentPoint(data.MovePointName);
			}
			else
			{
				npc_interface->StartPatrol();
			}
		}
	}
}

void UERGameInstance::SaveFieldObject(const TScriptInterface<IFieldObject_Interface>& FieldObject_Interface)
{
	CHECK_INVALID_PTR(FieldObject_Interface.GetObject())
	
	TWeakObjectPtr<UERSaveData> game_data = GetGameData();
	CHECK_INVALID_SMART_PTR(game_data)
	game_data->SetFieldObject(FieldObject_Interface);
	
	if (false == UGameplayStatics::SaveGameToSlot(game_data.Get(), FString("SavedGame"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Error"));
	}
}

void UERGameInstance::LoadFieldObject()
{
	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)
	
	TArray<AActor*> field_objects;
	UGameplayStatics::GetAllActorsWithTag(this, FName("FieldObject"), field_objects);
	for (AActor* const object : field_objects)
	{
		object->LifeSpanExpired();
	}
	
	if (UGameplayStatics::DoesSaveGameExist(FString("SavedGame"), 0))
	{
		UERSaveData* loaded_data = Cast<UERSaveData>(UGameplayStatics::LoadGameFromSlot(FString("SavedGame"), 0));
		CHECK_INVALID_PTR(loaded_data)
	
		TArray<FGameFieldObjectSaveData> datas = loaded_data->GetFieldObjects();
		for (const FGameFieldObjectSaveData& data : datas)
		{
			FActorSpawnParameters param;
			param.bNoFail = true;
	
			AFieldObject* object = world->SpawnActor<AFieldObject>(data.ClassType, data.Transform, param);
			CHECK_INVALID_PTR(object)

#if WITH_EDITOR
			object->SetFolderPath(FName("FieldObject"));
#endif
			IFieldObject_Interface* object_interface = Cast<IFieldObject_Interface>(object);
			CHECK_INVALID_PTR(object_interface)
	
			object_interface->SetFieldObjectTransform(data.Transform);
		}
	}
}

void UERGameInstance::SaveInteractObject(const TScriptInterface<IInteractor_Interface>& Interactor_Interface)
{
	CHECK_INVALID_PTR(Interactor_Interface.GetObject())

	TWeakObjectPtr<UERSaveData> game_data = GetGameData();
	CHECK_INVALID_SMART_PTR(game_data)
	game_data->SetInteractObject(Interactor_Interface);

	if (false == UGameplayStatics::SaveGameToSlot(game_data.Get(), FString("SavedGame"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Error"));
	}
}

void UERGameInstance::LoadInteractObject()
{
	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	TArray<AActor*> objects;
	TMap<FString, IInteractor_Interface*> interact_objects;
	UGameplayStatics::GetAllActorsWithTag(this, FName("InteractObject"), objects);
	for (AActor* const object : objects)
	{
		IInteractor_Interface* interactor = Cast<IInteractor_Interface>(object);
		if (!interactor) continue;

		interactor->SetFirstInteract(true);
		interact_objects.Add(interactor->GetInteractorName(), interactor);
	}

	if (interact_objects.IsEmpty()) return;

	if (UGameplayStatics::DoesSaveGameExist(FString("SavedGame"), 0))
	{
		UERSaveData* loaded_data = Cast<UERSaveData>(UGameplayStatics::LoadGameFromSlot(FString("SavedGame"), 0));
		CHECK_INVALID_PTR(loaded_data)

		TArray<FGameInteractorObjectSaveData> datas = loaded_data->GetInteractors();
		for (const FGameInteractorObjectSaveData& data : datas)
		{
			if (!interact_objects.Contains(data.InteractorName)) continue;

			IInteractor_Interface* interactor = *interact_objects.Find(data.InteractorName);
			if (!interactor) continue;

			interactor->SetFirstInteract(data.IsFirstInteract);
		}
	}
}

void UERGameInstance::SaveTutorialObject(const TScriptInterface<ITutorial_Interface>& Tutorial_Interface)
{
	CHECK_INVALID_PTR(Tutorial_Interface.GetObject())

	TWeakObjectPtr<UERSaveData> game_data = GetGameData();
	CHECK_INVALID_SMART_PTR(game_data)
	game_data->SetTutorialObject(Tutorial_Interface);

	if (false == UGameplayStatics::SaveGameToSlot(game_data.Get(), FString("SavedGame"), 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Error"));
	}
}

void UERGameInstance::LoadTutorialObject()
{
	UWorld* world = GetWorld();
	CHECK_INVALID_PTR(world)

	TArray<AActor*> tutorial_objects;
	UGameplayStatics::GetAllActorsWithTag(this, FName("TutorialObject"), tutorial_objects);
	for (AActor* const object : tutorial_objects)
	{
		object->LifeSpanExpired();
	}

	if (UGameplayStatics::DoesSaveGameExist(FString("SavedGame"), 0))
	{
		UERSaveData* loaded_data = Cast<UERSaveData>(UGameplayStatics::LoadGameFromSlot(FString("SavedGame"), 0));
		CHECK_INVALID_PTR(loaded_data)

		TArray<FGameTutorialSaveData> datas = loaded_data->GetTutorials();
		for (const FGameTutorialSaveData& data : datas)
		{
			FActorSpawnParameters param;
			param.bNoFail = true;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ATutorialVolumeBox* object = world->SpawnActor<ATutorialVolumeBox>(data.ClassType, data.Transform, param);
			CHECK_INVALID_PTR(object)

#if WITH_EDITOR
			object->SetFolderPath(FName("TutorialVolumes"));
#endif
			ITutorial_Interface* object_interface = Cast<ITutorial_Interface>(object);
			CHECK_INVALID_PTR(object_interface)

			object_interface->SetObjectTransform(data.Transform);
			object_interface->SetCallOut(data.TutorialDatas);
		}
	}
}

void UERGameInstance::ResetSaveData()
{
	UERSaveData* game_data;
	if (UGameplayStatics::DoesSaveGameExist(FString("SavedGame"), 0))
	{
		game_data = Cast<UERSaveData>(UGameplayStatics::LoadGameFromSlot(FString("SavedGame"), 0));
		game_data->ResetNpcObject();
		game_data->ResetFieldObject();
		game_data->ResetInteractor();
		game_data->ResetTutorial();

		if (false == UGameplayStatics::SaveGameToSlot(game_data, FString("SavedGame"), 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Save Error"));
		}
	}
}

const TWeakObjectPtr<UERSaveData> UERGameInstance::GetGameData()
{
	TWeakObjectPtr<UERSaveData> game_data;
	if (UGameplayStatics::DoesSaveGameExist(FString("SavedGame"), 0))
	{
		game_data = Cast<UERSaveData>(UGameplayStatics::LoadGameFromSlot(FString("SavedGame"), 0));
	}
	else
	{
		game_data = Cast<UERSaveData>(UGameplayStatics::CreateSaveGameObject(UERSaveData::StaticClass()));
	}

	return game_data;
}


