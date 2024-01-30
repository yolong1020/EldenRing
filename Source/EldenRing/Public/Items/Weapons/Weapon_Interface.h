// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/CharacterTypes.h"
#include "Items/Weapons/WeaponTypes.h"
#include "Weapon_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeapon_Interface : public UInterface
{
	GENERATED_BODY()
};

class ELDENRING_API IWeapon_Interface
{
	GENERATED_BODY()

public:
	virtual void SetWeaponCollision(const ECollisionEnabled::Type& Type) PURE_VIRTUAL(IWeapon_Interface::SetWeaponCollision,);
	virtual void SetAttackWeight(const EEquipState& equip_state, const EAttackStrength& attack_strength, const EAttackType& attack_type) PURE_VIRTUAL(IWeapon_Interface::SetAttackWeight, );
	virtual void ForeceSetAttackWeight(const EAttackWeight& Type) PURE_VIRTUAL(IWeapon_Interface::ForeceSetAttackWeight, );

	virtual void SetShieldMod(const bool& IsGuard) PURE_VIRTUAL(IWeapon_Interface::SetShieldMod,);
};
