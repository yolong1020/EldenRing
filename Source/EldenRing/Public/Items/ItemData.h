#pragma once

#include "Engine/DataTable.h"
#include "Equipment/EquipmentTypes.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	explicit FItemData() :
		ItemName(""), ItemDescription(""),
		EquipmentType(EEquipmentType::EET_None),
		SlotSize(FIntPoint::ZeroValue),
		Cost(0),
		BluePrintPath(""), MeshPath(""), IconPath(""), IconPathRotated("")
		{}
	~FItemData() {}

	bool operator==(const FItemData& Other) const
	{
		return  (ItemName == Other.ItemName) &&
				(ItemDescription == Other.ItemDescription) &&
				(EquipmentType == Other.EquipmentType) &&
				(SlotSize == Other.SlotSize) &&
				(BluePrintPath == Other.BluePrintPath);
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint SlotSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BluePrintPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPathRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> HideMaterials;
};