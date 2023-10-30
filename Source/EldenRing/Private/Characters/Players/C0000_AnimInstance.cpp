// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Players/C0000_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Characters/Players/C0000.h"
#include "Misc/DateTime.h"
#include "../DebugMacros.h"

void UC0000_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_character_C000 = Cast<AC0000>(TryGetPawnOwner());
	if (nullptr == m_character_C000) return;

	m_movement_component = m_character_C000->GetCharacterMovement();
}

void UC0000_AnimInstance::NativeUpdateAnimation(float delta)
{
	Super::NativeUpdateAnimation(delta);

	CHECK_INVALID(m_movement_component)
	CHECK_INVALID(m_character_C000)
	
	const FVector last_input_vector = m_movement_component->GetLastInputVector();
	AActor*	const actor_target	= m_character_C000->GetTarget();

	EActionState action = m_character_C000->GetActionState();

	if (action != EActionState::EAS_Knockbacking && action != EActionState::EAS_Consume)
	{
		bool is_action_guard	= (action == EActionState::EAS_Guarding);
		m_is_guard		= m_character_C000->IsInputKeyByFKey(EKeys::RightMouseButton) && is_action_guard;
		m_is_blend_by_bone	= IsTryingMove() && is_action_guard;
	}
	if (action == EActionState::EAS_Knockbacking) { m_is_blend_by_bone = false; }

	m_is_front_or_right	= (last_input_vector.Size() > 0) ? true : m_is_front_or_right;
	m_spine_rotation	= FRotator::ZeroRotator;

	bool enable_lock_on = (actor_target != nullptr && m_character_C000->GetLockOnState() == ELockOnState::ELOS_LockOn);
	enable_lock_on ? LockOnTarget(last_input_vector, actor_target, delta) : SetRotateAttack(delta);

	m_time_delta += delta;
	CheckNotInput(last_input_vector);
	CheckEnableSprintTurn();

	m_ground_speed		= FMath::Clamp(last_input_vector.Size(), 0, 1) * m_movement_component->MaxWalkSpeed;
	m_is_falling		= m_movement_component->IsFalling();
	m_equip_state		= m_character_C000->GetEquipState();
	m_lockon_rotation	= FMath::Lerp(m_lockon_rotation, m_spine_rotation, delta * 10);
}

void UC0000_AnimInstance::ResetEnableSprintTurn()
{
	m_time_cur_sprint_turn	= m_time_delta;
	m_enable_sprint_turn	= false;
}

void UC0000_AnimInstance::LockOnTarget(const FVector& last_input_vector, AActor* const actor_target, const float& delta)
{
	if (m_character_C000->GetGroundState() == EGroundState::EGS_Sprint) 
	{
		m_only_move_side = false;
		return; 
	}

	float input_scale = 0;
	if	(m_character_C000->IsInputKey(FName("MoveForward"), input_scale, true))	{ m_only_move_side = false; }
	else if (m_character_C000->IsInputKey(FName("MoveSide"), input_scale, true))	{ m_only_move_side = true; }

	if (input_scale != 0) { m_is_front_or_right = (input_scale > 0); }

	FVector direction_rotated;
	// Rotate Pelvis	-------------------------------------------
	if (0 != last_input_vector.Size() && m_character_C000->GetGroundState() > EGroundState::EGS_Unoccupied)
	{
		FRotator input_rotate = last_input_vector.GetSafeNormal2D().Rotation();

		if (m_only_move_side)		{ input_rotate.Yaw -= (90.f * input_scale); }
		else if (0 > input_scale)	{ input_rotate.Yaw += 180.f; }

		direction_rotated		= FRotationMatrix(input_rotate).GetUnitAxis(EAxis::X);
		const FRotator	rotate_calc	= UKismetMathLibrary::RInterpTo(m_character_C000->GetActorRotation(), direction_rotated.Rotation(), delta, 10);

		m_character_C000->SetActorRotation(rotate_calc.Quaternion());
	}

	// Rotate Spine	-------------------------------------------
	const FRotator player_rotator = UKismetMathLibrary::MakeRotFromX(m_character_C000->GetActorForwardVector());
	const FRotator target_rotator = UKismetMathLibrary::FindLookAtRotation(m_character_C000->GetActorLocation(), actor_target->GetActorLocation());
	m_spine_rotation 	      = target_rotator - player_rotator;

	if (20.f > FMath::Abs(m_spine_rotation.Yaw) || 45.f < FMath::Abs(m_spine_rotation.Yaw)) m_spine_rotation = FRotator::ZeroRotator;
}

void UC0000_AnimInstance::CheckNotInput(const FVector& last_input_vector)
{
	if (last_input_vector.Size() > 0) 
	{
		m_time_cur_input = m_time_delta;
		m_is_stop_move	 = false;
	}
	else if ((m_time_delta > m_time_cur_input + 0.1) && (EGroundState::EGS_Walk < m_character_C000->GetGroundState())) 
	{
		EActionState action = m_character_C000->GetActionState();
		if (EActionState::EAS_Dodgeing == action || EActionState::EAS_Guarding == action) return;

		m_time_cur_input = m_time_delta;
		m_is_stop_move	 = true;

		m_character_C000->SetGroundUnoccupied();
	}
}

void UC0000_AnimInstance::CheckEnableSprintTurn()
{
	if (m_character_C000->GetGroundState() != EGroundState::EGS_Sprint)
	{ 
		ResetEnableSprintTurn();
	}
	else if (m_time_delta > m_time_cur_sprint_turn + 0.5)
	{
		m_enable_sprint_turn = true;
	}
}

void UC0000_AnimInstance::SetRotateAttack(const float& delta)
{
	m_only_move_side = false;

	if (false == m_character_C000->GetInputEnableAttackRotate() ||
		m_character_C000->GetActionState() != EActionState::EAS_Attacking)
	{ return; }

	float	axis_x, axis_y  = 0;
	bool	move_axis_x 	= m_character_C000->IsInputKey(FName("MoveForward"), axis_x, true);
	bool	move_axis_y 	= m_character_C000->IsInputKey(FName("MoveSide"), axis_y, true);

	if (false == move_axis_x && false == move_axis_y) return;

	const FRotator control_rotation	= m_character_C000->GetControlRotation();

	const FRotator Yaw_rotation(0.f, control_rotation.Yaw, 0.f);
	const FVector  direction_x = FRotationMatrix(Yaw_rotation).GetUnitAxis(EAxis::X) * axis_x;
	const FVector  direction_y = FRotationMatrix(Yaw_rotation).GetUnitAxis(EAxis::Y) * axis_y;

	const FRotator rotate_calc = UKismetMathLibrary::RInterpTo(m_character_C000->GetActorRotation(), (direction_x + direction_y).Rotation(), delta, 10);

	m_character_C000->SetActorRotation(rotate_calc.Quaternion());
}

bool UC0000_AnimInstance::IsTryingMove()
{
	float scale = 0;
	if (m_character_C000->IsInputKey(FName("MoveForward"), scale, true)) { return true; }
	if (m_character_C000->IsInputKey(FName("MoveSide"), scale, true))    { return true; }

	return false;
}

void UC0000_AnimInstance::AnimNotify_MontageStart()
{

	if (RootMotionMode == ERootMotionMode::RootMotionFromEverything) 
	{ 
		SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
		
		if (nullptr == m_movement_component) return;
		m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}

}

void UC0000_AnimInstance::AnimNotify_MontageEnd()
{

	if (RootMotionMode == ERootMotionMode::RootMotionFromMontagesOnly)
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

		if (nullptr == m_movement_component) return;
		m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}

}

void UC0000_AnimInstance::AnimNotify_EnableBoxCollision()
{
	if (nullptr == m_character_C000) return;
	m_character_C000->SetWeaponCollision(ECollisionEnabled::Type::QueryOnly);
}

void UC0000_AnimInstance::AnimNotify_DisableBoxCollision()
{
	if (nullptr == m_character_C000) return;
	m_character_C000->SetWeaponCollision(ECollisionEnabled::Type::NoCollision);
}

void UC0000_AnimInstance::AnimNotify_JumpStart()
{
	if (nullptr == m_movement_component || nullptr == m_character_C000) return;

	m_movement_component->SetMovementMode(EMovementMode::MOVE_Flying);
	m_movement_component->bNotifyApex = true;

	m_character_C000->SetJumpStartLocation(m_character_C000->GetActorLocation());
}

void UC0000_AnimInstance::AnimNotify_OnLandEnd()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetLandingFinish();
}

void UC0000_AnimInstance::AnimNotify_OnDodgeEnd()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetActionUnoccupied();

	if (m_character_C000->IsInputAxisActionKey()) { m_character_C000->OnDodgeEnd(); }
}

void UC0000_AnimInstance::AnimNotify_OnTurnEnd()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnEndTurn();
}

void UC0000_AnimInstance::AnimNotify_OnApex()
{
	if (nullptr == m_movement_component) return;

	m_character_C000->GetJumpDirectionLength(m_character_C000->GetActorLocation());

	SetRootMotionMode(ERootMotionMode::NoRootMotionExtraction);
	m_movement_component->SetMovementMode(EMovementMode::MOVE_Falling);

	m_movement_component->bNotifyApex = false;
	m_movement_component->NotifyJumpApex();
}

void UC0000_AnimInstance::AnimNotify_Swap()
{
	CHECK_INVALID(m_character_C000)
	EWeaponEquipHand hand_type = m_character_C000->GetSwapHand();

	m_character_C000->Swap_Weapon(hand_type);
}

void UC0000_AnimInstance::AnimNotify_SwapEnd()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetActionUnoccupied();
}

void UC0000_AnimInstance::AnimNotify_ReserveNextAttack()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetAttackEnable(true);
}

void UC0000_AnimInstance::AnimNotify_OnAttackEnd()
{
	CHECK_INVALID(m_character_C000)
	if (EActionState::EAS_Execution == m_character_C000->GetActionState()) { m_character_C000->OnEndDirection(); }

	m_character_C000->OnEndAttack();

}

void UC0000_AnimInstance::AnimNotify_OnEnableInput()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetInputMovementEnable(true);
	m_character_C000->SetAttackEnable(true);
}

void UC0000_AnimInstance::AnimNotify_OnDisableInput()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetInputMovementEnable(false);
	m_character_C000->SetInputAttackRotateEnable(false);

	m_time_cur_input = m_time_delta;
	m_is_stop_move	 = false;
}

void UC0000_AnimInstance::AnimNotify_OnMoveStopEnd()
{
	CHECK_INVALID(m_character_C000)
	m_is_stop_move = false;
}

void UC0000_AnimInstance::AnimNotify_OnGuardStart()
{
	m_is_guard = true;
}

void UC0000_AnimInstance::AnimNotify_OnGuardEnd()
{
	m_is_guard = false;
	CHECK_INVALID(m_character_C000)

	if (false == m_character_C000->IsInputAxisActionKey()) { m_character_C000->OnGuardEnd(); }
}

void UC0000_AnimInstance::AnimNotify_OnKnockbackStart()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnStartKnockback();

	m_time_cur_input = m_time_delta;
	m_is_stop_move	 = false;
}

void UC0000_AnimInstance::AnimNotify_OnKnockbackEnd()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnEndKnockback();
}

void UC0000_AnimInstance::AnimNotify_OnEnableParryCollision()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetParryCollision(ECollisionEnabled::QueryOnly);
}

void UC0000_AnimInstance::AnimNotify_OnDisableParryCollision()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetParryCollision(ECollisionEnabled::NoCollision);
}

void UC0000_AnimInstance::AnimNotify_OnParryStart()
{
	CHECK_INVALID(m_character_C000)

	m_character_C000->SetInputMovementEnable(false);
	m_character_C000->SetAttackEnable(false);

	m_time_cur_input = m_time_delta;
	m_is_stop_move	 = false;
}

void UC0000_AnimInstance::AnimNotify_OnParryEnd()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnEndAttack();
	m_character_C000->SetInputMovementEnable(true);
	m_character_C000->SetAttackEnable(true);

	m_time_cur_input = m_time_delta;
	m_is_stop_move	 = false;
}

void UC0000_AnimInstance::AnimNotify_OnCameraShakeIn()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnCameraShakeIn();
}

void UC0000_AnimInstance::AnimNotify_OnCameraShakeOut()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnCameraShakeOut();
}

void UC0000_AnimInstance::AnimNotify_OnEnableShortAttack()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetShortAttackEnable(true);
}

void UC0000_AnimInstance::AnimNotify_OnDisableShortAttack()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->SetShortAttackEnable(false);
}

void UC0000_AnimInstance::AnimNotify_OnSuccessConsume()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnSuccessConsume();
}

void UC0000_AnimInstance::AnimNotify_OnEndConsume()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnEndConsume();
}

void UC0000_AnimInstance::AnimNotify_OnReactEnd()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnReactEnd();
}

void UC0000_AnimInstance::AnimNotify_OnUseStamina()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnUseStamina(50);
}

void UC0000_AnimInstance::AnimNotify_OnUseStaminaShort()
{
	CHECK_INVALID(m_character_C000)
	m_character_C000->OnUseStamina(30);
}
