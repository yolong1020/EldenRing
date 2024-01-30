// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquipmentTypes.h"
#include "ItemData.h"
#include "UI/Structure/InventoryTile.h"
#include "UI/UITypes.h"
#include "ItemObject.generated.h"

UCLASS()
class ELDENRING_API UItemObject : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override final;

	const FIntPoint GetDimension() const;

	void RotateItemObject();
	void SetItemProperties(const FName& RowName, const FItemData& Data, TSubclassOf<class AItem> Class, bool IsRotated);

	FORCEINLINE const FName&								GetRowName() const					{ return ItemRowName; }
	FORCEINLINE const EEquipmentType&						GetItemEquipType() const			{ return ItemData.EquipmentType; }
	FORCEINLINE const EDragAndDropHUD&						GetCurrentHUD() const				{ return CurrentHUD; }
	FORCEINLINE const FInventoryTile&						GetLastInventoryLocation() const	{ return LastInventoryLocation; }
	FORCEINLINE const FString&								GetLastEquipSlotName() const		{ return LastEquipSlotName; }
	FORCEINLINE const FItemData*							GetItemData() const					{ return &ItemData; }
	FORCEINLINE TObjectPtr<class UMaterialInterface>		GetIconImage(bool is_rotated) const	{ return is_rotated ? Icon_Rotated : Icon; }
	FORCEINLINE TSubclassOf<class AItem>					GetItemClass() const				{ return ItemClass; }
	FORCEINLINE const bool									IsRotated()	const					{ return IsRotatedItem; }
	FORCEINLINE const bool									IsEquiped()	const					{ return IsEquipedItem; }

	FORCEINLINE void SetEquipState(const bool& IsEquiped)						{ IsEquipedItem = IsEquiped; }
	FORCEINLINE void SetCurrentHUD(const EDragAndDropHUD& Type)					{ CurrentHUD = Type; }
	FORCEINLINE void SetLastInventoryLocation(const FInventoryTile& StartTile)  { LastInventoryLocation = StartTile; }
	FORCEINLINE void SetLastEquipSlotName(const FString& Name)					{ LastEquipSlotName = Name; }

private:
	UPROPERTY()
	TObjectPtr<class UMaterialInterface> Icon;

	UPROPERTY()
	TObjectPtr<class UMaterialInterface> Icon_Rotated;

	UPROPERTY()
	TSubclassOf<class AItem>	ItemClass;

	FItemData					ItemData;

	FName						ItemRowName;
	FIntPoint					ItemSlotSize;
	FInventoryTile				LastInventoryLocation;
	FString						LastEquipSlotName;
	EDragAndDropHUD				CurrentHUD;

	bool						IsRotatedItem;
	bool						IsEquipedItem;
};
