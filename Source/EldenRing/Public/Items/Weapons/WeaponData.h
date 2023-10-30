#pragma once

#include "Engine/DataTable.h"
#include "WeaponTypes.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPhysicalAttackType PhysicalAttType = EPhysicalAttackType::EPAT_Standard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EAttackWeight> Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EAttackWeight> DashAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EAttackWeight> JumpAttack;
};
