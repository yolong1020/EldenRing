#pragma once

#include "Engine/DataTable.h"
#include "ItemData.h"
#include "ItemVendor.generated.h"

USTRUCT(BlueprintType)
struct ELDENRING_API FItemVendor : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ItemList;
};