// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../CharacterTypes.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class AWeapon_Actor;
class UAnimMontage;

UCLASS()
class BASIC_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type type);

protected:
	virtual void BeginPlay() override;

protected:
	// Callbacks
	void MoveForward(float value);
	void MoveSide(float value);
	void LookUp(float value);
	void Turn(float value);
	void Interaction();
	void Attack();

	// Play Montage Func
	void PlayAttackMontage();
	void PlayEquipMontage(FName section_name);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool EnableAttack();
	
	bool CanUnequip();
	bool CanEquip();

	UFUNCTION(BlueprintCallable)
	void UnEquip();

	UFUNCTION(BlueprintCallable)
	void Equip();

	UFUNCTION(BlueprintCallable)
	void FinishEquip();

private:
	EEquipState m_equip_state	= EEquipState::EES_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState	m_action_state		= EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* m_spring_arm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_view_camera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* m_hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* m_eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* m_overlapping_item;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon_Actor* m_equiped_weapon;

/// <summary>
/// Animation Montages
/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_attack = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_equip = nullptr;

public:
	FORCEINLINE void SetOverlappingItem(AItem* item) { m_overlapping_item = item; }
	FORCEINLINE EEquipState GetEquipState() { return m_equip_state; }
};
