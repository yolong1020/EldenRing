// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveDatas.h"
#include "Characters/GameCharacter_Interface.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "FieldObject/FieldObject_Interface.h"
#include "Common/Interaction/Interactor_Interface.h"
#include "UI/Tutorial/Tutorial_Interface.h"
#include "ERSaveData.generated.h"

UCLASS()
class ELDENRING_API UERSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	void SetEquipedItems(const TArray<TObjectPtr<class UItemObject>>& Items);
	void SetInventoryItems(const TArray<TObjectPtr<class UItemObject>>& Items);
	void SetNpcObject(const TScriptInterface<INPC_Interfaces>& NPC_Interface);
	void SetFieldObject(const TScriptInterface<IFieldObject_Interface>& FieldObject_Interface);
	void SetInteractObject(const TScriptInterface<IInteractor_Interface>& Interactor_Interface);
	void SetTutorialObject(const TScriptInterface<ITutorial_Interface>& Tutorial_Interface);

	FORCEINLINE void SetStartPoint(const FTransform& Point)						{ m_start_point = Point; }
	FORCEINLINE void SetCharacterGold(const int32& TotalGold)					{ m_player_gold = TotalGold; }

	FORCEINLINE const TArray<FEquipSaveData>& GetEquipedItems()					{ return m_equiped_items; }
	FORCEINLINE const TArray<FInventorySaveData>& GetInventoryItems()			{ return m_inventory_items; }
	FORCEINLINE const TArray<FGameObjectSaveData>& GetNPCs()					{ return m_npcs; }
	FORCEINLINE const TArray<FGameFieldObjectSaveData>& GetFieldObjects()		{ return m_field_objects; }
	FORCEINLINE const TArray<FGameInteractorObjectSaveData>& GetInteractors()	{ return m_interactor_objects; }
	FORCEINLINE const FTransform& GetStartPoint()								{ return m_start_point; }
	FORCEINLINE const TArray<FGameTutorialSaveData>& GetTutorials()				{ return m_tutorial_objects; }

	void ResetNpcObject()	{ m_npcs.Empty(); }
	void ResetFieldObject() { m_field_objects.Empty(); }
	void ResetInteractor()	{ m_interactor_objects.Empty(); }
	void ResetTutorial()	{ m_tutorial_objects.Empty(); }

private:
	UPROPERTY()
	FTransform m_start_point;

	UPROPERTY()
	TArray<FEquipSaveData> m_equiped_items;

	UPROPERTY()
	TArray<FInventorySaveData> m_inventory_items;

	UPROPERTY()
	TArray<FGameObjectSaveData> m_npcs;

	UPROPERTY()
	TArray<FGameFieldObjectSaveData> m_field_objects;

	UPROPERTY()
	TArray<FGameInteractorObjectSaveData> m_interactor_objects;

	UPROPERTY()
	TArray<FGameTutorialSaveData> m_tutorial_objects;

	int32	m_player_gold;
	bool	m_potion_guide;
};
