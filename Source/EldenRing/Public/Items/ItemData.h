#pragma once

#include "Engine/DataTable.h"
#include "Equipment/EquipmentTypes.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint	SlotSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPathRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> HideMaterials;
};