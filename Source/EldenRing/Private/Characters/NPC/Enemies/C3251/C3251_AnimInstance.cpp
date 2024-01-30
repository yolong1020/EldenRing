// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/C3251_AnimInstance.h"
#include "Characters/NPC/Enemies/C3251/C3251.h"
#include "Characters/NPC/Enemies/C3251/C3251_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Macro/DebugMacros.h"

void UC3251_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_character = Cast<AC3251>(GetOwningActor());
	CHECK_INVALID_PTR(m_character)

	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(m_character);

	m_controller = Cast<AC3251_Controller>(m_character->GetController());
	CHECK_INVALID_PTR(m_controller)

	m_need_rotate = false;
}

void UC3251_AnimInstance::NativeUpdateAnimation(float delta)
{
	Super::NativeUpdateAnimation(delta);

	if (!m_character) return;
	m_action_state = (EActionState_NPC)m_character->GetActionState();
	INPC_Interfaces* npc_interface = Cast<INPC_Interfaces>(m_character);

	if (m_need_rotate && npc_interface)
	{
		float offset = 0, speed = 0;
		npc_interface->GetOffsetDegreeAndInterptSpeed(offset, speed);
		FVector  dir  = m_character->GetTargetLocation() - m_character->GetActorLocation();
		FRotator goal = (FQuat(FRotator(0.f, -offset, 0.f).Quaternion()) * dir).Rotation();

		FQuat calc = FQuat::Slerp(m_character->GetActorRotation().Quaternion(), goal.Quaternion(), speed * delta);
		m_character->SetActorRotation(calc);
	}
}

void UC3251_AnimInstance::AnimNotify_OnRootMotion()
{
	if (RootMotionMode != ERootMotionMode::RootMotionFromEverything)
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	}
}

void UC3251_AnimInstance::AnimNotify_OnJump()
{
	CHECK_INVALID_PTR(m_character)
	UCharacterMovementComponent* movement = m_character->GetCharacterMovement();
	CHECK_INVALID_PTR(movement)

	movement->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UC3251_AnimInstance::AnimNotify_OnEndJump()
{
	CHECK_INVALID_PTR(m_character)
	UCharacterMovementComponent* movement = m_character->GetCharacterMovement();
	CHECK_INVALID_PTR(movement)

	movement->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UC3251_AnimInstance::AnimNotify_OnLockRotation()
{
	CHECK_INVALID_PTR(m_character)
	m_character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
}

void UC3251_AnimInstance::AnimNotify_OnUnlockRotation()
{
	CHECK_INVALID_PTR(m_character)
	m_character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
}

void UC3251_AnimInstance::AnimNotify_OnShowDirecting()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnShowDirecting();
}

void UC3251_AnimInstance::AnimNotify_OnBattle()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnBattle();
}

void UC3251_AnimInstance::AnimNotify_OnEndAttack()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnEndAttack();
}

void UC3251_AnimInstance::AnimNotify_OnEndMove()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnEndMove();
}

void UC3251_AnimInstance::AnimNotify_OnInputCombo()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnInputCombo();
}

void UC3251_AnimInstance::AnimNotify_OnRotate()
{
	m_need_rotate = true;
	CHECK_INVALID_PTR(m_character)
	m_character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
}

void UC3251_AnimInstance::AnimNotify_OnEndRotate()
{
	m_need_rotate = false;
}

void UC3251_AnimInstance::AnimNotify_OnRotateMoving()
{
	CHECK_INVALID_PTR(m_character)
	m_character->StartRotateMoving();
}

void UC3251_AnimInstance::AnimNotify_EnableWeaponCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetWeaponCollision(ECollisionEnabled::QueryOnly);
}

void UC3251_AnimInstance::AnimNotify_DisableWeaponCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetWeaponCollision(ECollisionEnabled::NoCollision);
}

void UC3251_AnimInstance::AnimNotify_EnableBodyCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetBodyCollision(ECollisionEnabled::QueryOnly);
}

void UC3251_AnimInstance::AnimNotify_DisableBodyCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetBodyCollision(ECollisionEnabled::NoCollision);
}

void UC3251_AnimInstance::AnimNotify_EnableLegsCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetLegsCollision(ECollisionEnabled::QueryOnly);
}

void UC3251_AnimInstance::AnimNotify_DisableLegsCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetLegsCollision(ECollisionEnabled::NoCollision);
}

void UC3251_AnimInstance::AnimNotify_EnableShieldCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetShieldCollision(ECollisionEnabled::QueryAndPhysics);
}

void UC3251_AnimInstance::AnimNotify_DisableShieldCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetShieldCollision(ECollisionEnabled::NoCollision);
}

void UC3251_AnimInstance::AnimNotify_EnableBlendPerBone()
{
	m_is_blend_by_bone = true;
}

void UC3251_AnimInstance::AnimNotify_DisableBlendPerBone()
{
	m_is_blend_by_bone = false;
}
