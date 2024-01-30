// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Field/FieldSystemTypes.h"
#include "WeaponTypes.h"
#include "Characters/CharacterTypes.h"
#include "Shield_Actor.generated.h"

UCLASS()
class ELDENRING_API AShield_Actor : public AWeapon_Actor
{
	GENERATED_BODY()
	
public:
	AShield_Actor();
	virtual void SetWeaponCollision(const ECollisionEnabled::Type& Type) override;
	FORCEINLINE virtual void SetShieldMod(const bool& IsGuard) override { m_is_guard = IsGuard; }
	FORCEINLINE void ForeceSetAttackWeight(const EAttackWeight& Weight) { m_attack_weight = Weight; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnShieldOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool m_is_guard;
};
