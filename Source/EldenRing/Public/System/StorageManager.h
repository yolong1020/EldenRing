// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Structure/InventoryTile.h"
#include "Characters/GameCharacter_Interface.h"
#include "UI/UIDelegates.h"
#include "UI/UITypes.h"
#include "StorageManager.generated.h"

class UItemInfo;
class UItemObject;

UCLASS()
class ELDENRING_API UStorageManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void OpenInventory(const IGameCharacter_Interface* Character, const TWeakObjectPtr<class APlayerController>& Controller);

	FORCEINLINE const bool IsActiveInventory() const;
	FORCEINLINE TArray<TObjectPtr<UItemObject>>* const GetSlots() { return &m_slots; }
	FORCEINLINE const FIntVector2 GetSize() { return FIntVector2(m_colum_count, m_row_count); }

	void InitStorageWidget(class AC0000* const Player);
	void InitUpdateGoldCallback(const FHUDGoldUpdate& Callback);
	void InitUpdateGridCallback(const FRefreshInteractionGrid& Callback);
	void ShowItemInfo(const FVector2D& Position, const FVector2D& SlotSize, const TObjectPtr<UItemObject> ItemObject);
	void HideItemInfo();
	
	bool TryUpdateStorage();
	bool TryUpdateGold(const int32& TotalGold);
	bool TryAddItem(const TObjectPtr<UItemObject> ItemObject);
	bool TryAddItemAtIndex(const TObjectPtr<UItemObject> ItemObject, const int32& Index);
	bool TryAddItemAtTile(const TObjectPtr<UItemObject> ItemObject, const FInventoryTile& StartTile);
	bool TryRemoveItem(const TObjectPtr<UItemObject> ItemObject);
	void ClearStorage();

	const int32 HaveSpaceItem(const TObjectPtr<UItemObject> ItemObject) const;
	const bool IsEmptyTile(const TObjectPtr<UItemObject> ItemObject, const FInventoryTile& Tile) const;
	const bool IsEmptyIndex(const TObjectPtr<UItemObject> ItemObject, const int32& Index) const;
	const bool IsInInventory(const TObjectPtr<UItemObject> ItemObject) const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;
	
private:
	const FInventoryTile IndexToTile(const int32& Index) const;
	const int32			 TileToIndex(const FInventoryTile& Tile) const;

	void AddItem(TObjectPtr<UItemObject> ItemObject, const int32& Index);
	void RemoveItem(const TObjectPtr<UItemObject> ItemObject);

private:
	UPROPERTY()
	TArray<TObjectPtr<UItemObject>> m_slots;

	UPROPERTY()
	TWeakObjectPtr<class UUIManager> m_ui_mgr;

	UPROPERTY()
	TObjectPtr<class UInventory> m_widget_inventory;

	UPROPERTY()
	TObjectPtr<class UInventoryEquip> m_widget_equipment;

	UPROPERTY()
	TObjectPtr<class UInventoryGrid> m_widget_grid;

	UPROPERTY()
	TObjectPtr<UItemInfo> m_item_info;

	int32 m_colum_count;
	int32 m_row_count;
	float m_total_gold;

	FRefreshInteractionGrid m_update_delegate;
	FHUDGoldUpdate m_update_gold_delegate;
};