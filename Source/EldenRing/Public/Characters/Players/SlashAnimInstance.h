// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlashCharacter.h"
#include "../CharacterTypes.h"
#include "SlashAnimInstance.generated.h"

/**
 * 
 */
class ASlashCharacter;
class UCharacterMovementComponent;

UCLASS()
class BASIC_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float delta_time) override;

	UPROPERTY(BlueprintReadOnly)
	ASlashCharacter* m_slash_character = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* m_slash_movement = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float m_ground_speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_falling = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Equip State")
	EEquipState m_equip_state = EEquipState::EES_Unequipped;


};
