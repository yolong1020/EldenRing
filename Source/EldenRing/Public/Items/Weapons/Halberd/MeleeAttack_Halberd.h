// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/MeleeAttack_Actor.h"
#include "Items/Weapons/WeaponTypes.h"
#include "MeleeAttack_Halberd.generated.h"

UCLASS()
class ELDENRING_API AMeleeAttack_Halberd : public AMeleeAttack_Actor
{
	GENERATED_BODY()
	
public:
	AMeleeAttack_Halberd();
	virtual void SetWeaponCollision(const ECollisionEnabled::Type& Type) override;

	FORCEINLINE void ForeceSetAttackWeight(const EAttackWeight& Weight) { m_attack_weight = Weight; }

protected:
	virtual void BeginPlay() override;
};
