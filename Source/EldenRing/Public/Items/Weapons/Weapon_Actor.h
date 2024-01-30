// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Field/FieldSystemTypes.h"
#include "WeaponTypes.h"
#include "Characters/CharacterTypes.h"
#include "Items/Weapons/Weapon_Interface.h"
#include "Weapon_Actor.generated.h"

class USoundBase;
class UBoxComponent;
class UFieldSystemComponent;
class UFieldSystemMetaDataFilter;
class URadialFalloff;
class URadialVector;
class USkeletalMeshComponent;

struct FWeaponData;

UCLASS()
class ELDENRING_API AWeapon_Actor : public AItem, public IWeapon_Interface
{
	GENERATED_BODY()
	
public:	
	AWeapon_Actor();
	virtual void Tick(float DeltaTime) override;

	void Equip(USceneComponent* parent, FName socket_name, AActor* owner, APawn* instigator);
	virtual void SetAttackWeight(const EEquipState& equip_state, const EAttackStrength& attack_strength, const EAttackType& attack_type) override;
	virtual void FadeOutItem(const float Duration) override final;
	void AttachMashToSocket(USceneComponent* InParent, const FName& InSocketName);

	FORCEINLINE void SetWeaponUsing(const bool& is_using) { m_is_using = is_using; }

	FORCEINLINE const bool&				IsUsingWeapon()			{ return m_is_using; }
	FORCEINLINE const EWeaponType&		GetWeaponType()			{ return m_weapon_type; }
	FORCEINLINE const EAttackWeight&	GetAttackWeight()		{ return m_attack_weight; }
	FORCEINLINE FName					GetAttachSocketName()	{ return m_mesh->GetAttachSocketName(); }

	int32 GetWeaponDamage(const EAttackType& attack_type, const EAttackStrength& attack_strength = EAttackStrength::EATKS_None);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DestructableObjectAction(const FVector& field_location);
	bool IsEnableCollision(AActor* const OtherActor);

private:
	UFUNCTION()
	void UpdateFadeOutItem(const float& Value);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* m_mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_mesh_static;

	UPROPERTY()
	TArray<AActor*>  m_ignore_actors;

	FWeaponData*  m_weapon_data;
	EAttackWeight m_attack_weight;
	EAttackType	  m_owner_attack_type;

#pragma region Component
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* m_weapon_sound;

	UPROPERTY(VisibleAnywhere)
	UFieldSystemComponent* m_field_system;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	EWeaponType m_weapon_type;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float m_value_damage;

	bool m_is_using;
#pragma endregion

#pragma region Radial
	//	Falloff
	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties")
	float m_radial_falloff_magnitude = 0.f;

	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties")
	float m_radial_falloff_range_min = 0.f;

	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties")
	float m_radial_falloff_range_max = 0.f;

	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties")
	float m_radial_falloff_radius = 0.f;

	UPROPERTY(EditAnywhere, Category = "Radial Falloff Properties")
	TEnumAsByte<EFieldFalloffType> m_radial_falloff_type;

	//	Vector
	UPROPERTY(EditAnywhere, Category = "Radial Vector Properties")
	float m_radial_vector_magnitude = 0.f;
#pragma endregion
};
