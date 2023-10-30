// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../HUD_Delegates.h"
#include "InventoryComponent.generated.h"

class  UInventory;
class  UItemObject;
class  UInventoryEquip;
class  UInventoryGrid;
struct FInventoryTile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASIC_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

public:
	void  ToggleInventory();
	
	bool  TryAddItem(UItemObject* item_object);
	bool  TryAddItemAt(UItemObject* item_object, const int32& index);
	bool  TryAddItemAndEquip(UItemObject* item_object);

	const TMap<UItemObject*, FInventoryTile> GetAllItems();
	void RefreshGoldAmount(const int32& gold_amount);
	void CheckAndUnequipSlot(UItemObject* const item_object);

	UFUNCTION()
	void RemoveItem(UItemObject* item_object);
	UFUNCTION()
	void AddItemAtTile(UItemObject* item_object, const FInventoryTile& start_tile);
	UFUNCTION()
	void AddItemAtEmptySlot(UItemObject* item_object);

	FInventoryTile	IndexToTile(int32 index);
	int32			TileToIndex(const FInventoryTile& tile);

	UFUNCTION()
	bool IsRoomAvailableForPayload(UItemObject* item_object);
	bool IsRoomAvailable(UItemObject* item_object, const int32& index);
	bool IsInInventory(UItemObject* item_object);

	FORCEINLINE FItemOnRemoved&		GetItemRemoveDelegate()			{ return ItemOnRemoved; }
	FORCEINLINE FItemOnAdd&			GetItemAddDelegate()			{ return ItemOnAdd; }
	FORCEINLINE FItemOnAddAtEmpty&	GetItemAddAtEmptyDelegate()		{ return ItemOnAddAtEmpty; }
	FORCEINLINE const int32&		GetColumSize()					{ return m_colum_count; }
	FORCEINLINE const int32&		GetRowSize()					{ return m_row_count; }

protected:
	virtual void BeginPlay() override;

private:
	void AddItem(UItemObject* item_object, const int32& index);

private:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UInventory> m_widget_inventory_class;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TArray<UItemObject*> m_slots;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Properties", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	int32 m_colum_count;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Properties", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	int32 m_row_count;

	UPROPERTY()
	APlayerController*		m_player_controller;
	
	UPROPERTY()
	UInventory*				m_widget_inventory;

	UPROPERTY()
	UInventoryEquip*		m_widget_equipment;
	
	UPROPERTY()
	UInventoryGrid*			m_widget_grid;

	bool					m_is_opened;

	FItemOnAdd				ItemOnAdd;
	FItemOnAddAtEmpty		ItemOnAddAtEmpty;
	FItemOnRemoved			ItemOnRemoved;
};
