// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Players/SlashAnimInstance.h"
#include "Characters/Players/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_slash_character = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (nullptr != m_slash_character)
	{
		m_slash_movement = m_slash_character->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float delta_time)
{
	Super::NativeUpdateAnimation(delta_time);

	if (nullptr != m_slash_movement)
	{
		m_ground_speed = UKismetMathLibrary::VSizeXY(m_slash_movement->Velocity);
		m_is_falling = m_slash_movement->IsFalling();

		m_equip_state = m_slash_character->GetEquipState();
	}
}
