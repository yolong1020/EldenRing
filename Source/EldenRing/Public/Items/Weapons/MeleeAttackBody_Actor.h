// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon_Interface.h"
#include "Characters/CharacterTypes.h"
#include "Items/Weapons/WeaponTypes.h"
#include "Field/FieldSystemTypes.h"
#include "MeleeAttackBody_Actor.generated.h"

UCLASS()
class ELDENRING_API AMeleeAttackBody_Actor : public AActor, public IWeapon_Interface
{
	GENERATED_BODY()
	
public:
	explicit AMeleeAttackBody_Actor();

	void InitAttackBodyCollision(USceneComponent* const Parent, const FName& SocketName, const FVector& Location, const FRotator& Rotator, const float& Radius, const float& Height, AActor* OwnerActor, APawn* InstigatorPawn);
	virtual void SetWeaponCollision(const ECollisionEnabled::Type& Type) override;

protected:
	virtual void BeginPlay() override final;

private:
	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool IsEnableCollision(AActor* const OtherActor);

private:
	UPROPERTY(EditAnywhere, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* m_weapon_capsule;

	EAttackWeight m_attack_weight;
	EAttackType	  m_owner_attack_type;

#pragma region Component
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UFieldSystemComponent* m_field_system;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType m_weapon_type;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float m_value_damage;
#pragma endregion

#pragma region Radial
	//	Falloff
	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties", meta = (AllowPrivateAccess = "true"))
	float m_radial_falloff_magnitude = 0.f;

	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties", meta = (AllowPrivateAccess = "true"))
	float m_radial_falloff_range_min = 0.f;

	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties", meta = (AllowPrivateAccess = "true"))
	float m_radial_falloff_range_max = 0.f;

	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties", meta = (AllowPrivateAccess = "true"))
	float m_radial_falloff_radius = 0.f;

	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EFieldFalloffType> m_radial_falloff_type;

	//	Vector
	UPROPERTY(EditAnywhere, Category = "Radial Vector Properties", meta = (AllowPrivateAccess = "true"))
	float m_radial_vector_magnitude = 0.f;
#pragma endregion
};
