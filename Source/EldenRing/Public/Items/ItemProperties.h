#pragma once

#include "Engine/DataTable.h"
#include "ItemProperties.generated.h"

USTRUCT(BlueprintType)
struct FItemProperties : public FTableRowBase
{
	GENERATED_BODY()

	explicit FItemProperties() : 
		Attack(FVector::ZeroVector), 
		Shield(FVector::ZeroVector), 
		Hp(0), 
		Stamina(0), 
		Power(0) {}
	~FItemProperties() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Power;

};