// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Field/FieldSystemTypes.h"
#include "WeaponTypes.h"
#include "Characters/CharacterTypes.h"
#include "Shield_Actor.generated.h"

UCLASS()
class BASIC_API AShield_Actor : public AWeapon_Actor
{
	GENERATED_BODY()
	
public:
	AShield_Actor();
	virtual void Tick(float DeltaTime) override;

	void SetShieldCollision(const ECollisionEnabled::Type& type);

protected:
	virtual void BeginPlay() override;
};
