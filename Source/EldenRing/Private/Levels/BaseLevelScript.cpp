// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/BaseLevelScript.h"
#include "GameBase/ERGameMode.h"
#include "GameBase/ERGameInstance.h"
#include "System/SoundscapeManager.h"
#include "System/StorageManager.h"
#include "WorldPartition/WorldPartitionSubsystem.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"
#include "WorldPartition/DataLayer/ActorDataLayer.h"
#include "Kismet/GameplayStatics.h"
#include "Macro/DebugMacros.h"

void ABaseLevelScript::BeginPlay()
{
	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)

	m_sound_mgr = instance->GetSubsystem<USoundscapeManager>();
	CHECK_INVALID_SMART_PTR(m_sound_mgr)
}

void ABaseLevelScript::LoadDataLayer()
{
	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)
	bool is_new = instance->IsNewGame();

	AERGameMode* game_mode = Cast<AERGameMode>(UGameplayStatics::GetGameMode(this));
	CHECK_INVALID_PTR(game_mode)
	FActorDataLayer landscape_data = game_mode->GetLandScapeData();

	UDataLayerManager* layer_manager = UDataLayerManager::GetDataLayerManager(GetWorld());
	if (!layer_manager) return;

	const UDataLayerInstance* landscape_instance = layer_manager->GetDataLayerInstanceFromName(landscape_data.Name);
	CHECK_INVALID_PTR(landscape_instance)
	layer_manager->SetDataLayerInstanceRuntimeState(landscape_instance, EDataLayerRuntimeState::Activated);

	FActorDataLayer	object_data = is_new ? game_mode->GetDefaultData() : game_mode->GetFieldObjectData();
	const UDataLayerInstance* object_instance = layer_manager->GetDataLayerInstanceFromName(object_data.Name);
	CHECK_INVALID_PTR(object_instance)
	layer_manager->SetDataLayerInstanceRuntimeState(object_instance, EDataLayerRuntimeState::Activated);
}

void ABaseLevelScript::LoadNavDataLayer()
{
	AERGameMode* game_mode = Cast<AERGameMode>(UGameplayStatics::GetGameMode(this));
	CHECK_INVALID_PTR(game_mode)
	FActorDataLayer nav_data = game_mode->GetNavData();

	UDataLayerManager* layer_manager = UDataLayerManager::GetDataLayerManager(GetWorld());
	if (!layer_manager) return;

	const UDataLayerInstance* data_instance = layer_manager->GetDataLayerInstanceFromName(nav_data.Name);
	CHECK_INVALID_PTR(data_instance)
	layer_manager->SetDataLayerInstanceRuntimeState(data_instance, EDataLayerRuntimeState::Activated);
}
