#pragma once

#include "Characters/CharacterTypes.h"
#include "Items/Weapons/WeaponTypes.h"
#include "NPC_CombatActionData.generated.h"

USTRUCT(BlueprintType)
struct ELDENRING_API FNPC_CombatActionData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	explicit FNPC_CombatActionData() : 
		ActionID(""), 
		CombatActionType(ECombatAction_NPC::ECAN_End), 
		ActionDirection(EGameDirection::EGD_None),
		ActionBound(ECombatBound::ECB_None),
		AttackWeight(EAttackWeight::EAW_None),
		WeaponType(EWeaponType::EWT_StraightSword),
		OffsetDegree(0),
		InterptSpeed(1) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActionID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> DerivedAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatAction_NPC CombatActionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameDirection ActionDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECombatBound ActionBound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackWeight AttackWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetDegree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterptSpeed;
};

USTRUCT(BlueprintType)
struct ELDENRING_API FNPC_StartableComboActions : public FTableRowBase
{
	GENERATED_BODY()

public:
	explicit FNPC_StartableComboActions() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActionID;
};