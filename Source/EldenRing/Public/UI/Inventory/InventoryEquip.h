// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Equipment/EquipmentTypes.h"
#include "InventoryEquip.generated.h"

class UBorder;
class UInventoryEquipSlot;
class UStorageManager;
class UEquipmentManager;
class USkeletalMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ELDENRING_API UInventoryEquip : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitEquipWidget(class AC0000* const Player);

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
#pragma region Widget
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBorder> GridBorder;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryEquipSlot> HeadSlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryEquipSlot> BodySlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryEquipSlot> HandsSlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryEquipSlot> PantsSlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryEquipSlot> RightWeaponSlot;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryEquipSlot> LeftWeaponSlot;
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

	UPROPERTY()
	TWeakObjectPtr<UStorageManager>   m_storage_mgr;

	UPROPERTY()
	TWeakObjectPtr<UEquipmentManager> m_equipment_mgr;

	TArray<TTuple<USkeletalMeshComponent*, TObjectPtr<class UItemObject> >> m_equiped_item;
};
