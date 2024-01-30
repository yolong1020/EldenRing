// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/UIDelegates.h"
#include "Items/Equipment/EquipmentTypes.h"
#include "EquipmentManager.generated.h"

class UInventoryEquipSlot;

UCLASS()
class ELDENRING_API UEquipmentManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void SetEquipmentData(class AC0000* const Player, const TArray<UInventoryEquipSlot*>&& Slots);
	void InitEquipCallback(const FTryEquipItem& Callback);

	void UnequipAllSlot();
	bool TryEquipItem(const TObjectPtr<UItemObject> ItemObject, const EEquipmentType& Type, const bool IsRight = true);
	bool TryUnequipItem(const EEquipmentType& Type, const bool IsRight = true);
	UInventoryEquipSlot* const GetMatchingSlot(const TObjectPtr<UItemObject> ItemObject);
	UInventoryEquipSlot* const FindEquipSlot(const TObjectPtr<UItemObject> ItemObject);

	const TArray<TObjectPtr<UItemObject>> GetAllEquipedItems();
	TArray<TArray<UMaterialInstanceDynamic*>> GetAllArmorMaterials();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;

private:
	void UpdateMesh(const TObjectPtr<UItemObject> ItemObject, const EEquipmentType& Type, const bool IsRight = true);
	void MeshActivate(const TObjectPtr<UItemObject> ItemObject, const bool& IsShow);

private:
	UPROPERTY()
	TObjectPtr<class AC0000> m_player;
	
	UPROPERTY()
	TArray<TObjectPtr<UInventoryEquipSlot>> m_slots;

	TArray<TTuple<TObjectPtr<class USkeletalMeshComponent>, TObjectPtr<UItemObject>>> m_equipments;

	FTryEquipItem	 m_equip_delegate;
	FTryUnequipItem  m_unequip_delegate;
};
