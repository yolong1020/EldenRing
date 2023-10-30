// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Items/Equipment/EquipmentTypes.h"
#include "InventoryEquip.generated.h"

class UBorder;
class UInventoryComponent;
class UInventoryEquipSlot;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASIC_API UInventoryEquip : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitEquipWidget(UInventoryComponent* const inventory_component, const float& tile_size);

	void CheckAndUnequipSlot(UItemObject* const item_object);
	bool IsSlotEmpty(const EEquipmentType& equip_type);

	UInventoryEquipSlot* const GetSlotWidget(const EEquipmentType& equip_type);
	UInventoryEquipSlot* const GetEmptyWeaponSlotWidget();
	UInventoryEquipSlot* const GetMatchingItemSlot(const EEquipmentType& equip_type, UItemObject* const item_object);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* m_inventory_component;

#pragma region Widget
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UBorder* GridBorder;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UInventoryEquipSlot* HeadSlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UInventoryEquipSlot* BodySlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UInventoryEquipSlot* HandsSlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UInventoryEquipSlot* PantsSlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UInventoryEquipSlot* RightWeaponSlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UInventoryEquipSlot* LeftWeaponSlot;
#pragma endregion

#pragma region Slot Size
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector2D m_slot_size_head;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector2D m_slot_size_body;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector2D m_slot_size_hands;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector2D m_slot_size_pants;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FVector2D m_slot_size_weapon;
#pragma endregion
};
