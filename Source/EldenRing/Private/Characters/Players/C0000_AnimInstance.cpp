// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Players/C0000_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Characters/Players/C0000.h"
#include "Misc/DateTime.h"
#include "System/UIManager.h"
#include "Macro/DebugMacros.h"

void UC0000_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_character = Cast<AC0000>(TryGetPawnOwner());
	if (nullptr == m_character) return;

	m_movement_component = m_character->GetCharacterMovement();
}

void UC0000_AnimInstance::NativeUpdateAnimation(float Delta)
{
	Super::NativeUpdateAnimation(Delta);

	if (!m_movement_component || !m_character) return;
	
	const FVector last_input_vector = m_movement_component->GetLastInputVector();
	AActor*	const actor_target		= m_character->GetTarget();

	EActionState action = m_character->GetActionState();

	if (action != EActionState::EAS_Knockbacking && action != EActionState::EAS_Consume)
	{
		bool is_action_guard	= (action == EActionState::EAS_Guarding);
		m_is_guard				= m_character->IsInputKeyByFKey(EKeys::RightMouseButton) && is_action_guard;
		m_is_blend_by_bone		= IsTryingMove() && is_action_guard;
	}
	if (action == EActionState::EAS_Knockbacking) m_is_blend_by_bone = false;

	m_is_front_or_right	= (last_input_vector.Size() > 0) ? true : m_is_front_or_right;
	m_spine_rotation	= FRotator::ZeroRotator;

	bool enable_lock_on = (actor_target && m_character->GetLockOnState() == ELockOnState::ELOS_LockOn);
	enable_lock_on ? LockOnTarget(last_input_vector, actor_target, Delta) : SetRotateAttackByInput(Delta);

	m_time_delta += Delta;
	CheckNotInput(last_input_vector);
	CheckEnableSprintTurn();

	m_ground_speed		= FMath::Clamp(last_input_vector.Size(), 0, 1) * m_movement_component->MaxWalkSpeed;
	m_is_falling		= m_movement_component->IsFalling() && action != EActionState::EAS_HitReact;
	m_equip_state		= (EEquipState)m_character->GetWeaponEquipState();
	m_lockon_rotation	= FMath::Lerp(m_lockon_rotation, m_spine_rotation, Delta * 10);
}

void UC0000_AnimInstance::ResetEnableSprintTurn()
{
	m_time_cur_sprint_turn	= m_time_delta;
	m_enable_sprint_turn	= false;
}

void UC0000_AnimInstance::LockOnTarget(const FVector& LastInputVector, AActor* const Target, const float& Delta)
{
	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	UUIManager* ui_mgr = instance->GetSubsystem<UUIManager>();
	ui_mgr->ShowLockOn(Cast<APlayerController>(m_character->Controller), Cast<AGameCharacter>(Target));

	if (m_character->GetGroundState() == EGroundState::EGS_Sprint) 
	{
		m_only_move_side = false;
		return; 
	}

	if (m_character->GetActionState() == EActionState::EAS_Attacking)
	{
		SetRotateAttack(Delta);
		return;
	}

	float input_scale = 0;
	if		(m_character->IsInputKey(FName("MoveForward"), input_scale, true))	{ m_only_move_side = false; }
	else if (m_character->IsInputKey(FName("MoveSide"), input_scale, true))	{ m_only_move_side = true; }

	if (input_scale != 0) m_is_front_or_right = (input_scale > 0);

	FVector direction_rotated;
	// Rotate Pelvis	-------------------------------------------
	if (0 != LastInputVector.Size() && m_character->GetGroundState() > EGroundState::EGS_Unoccupied)
	{
		FRotator input_rotate = LastInputVector.GetSafeNormal2D().Rotation();

		if (m_only_move_side)	  input_rotate.Yaw -= (90.f * input_scale);
		else if (0 > input_scale) input_rotate.Yaw += 180.f;

		direction_rotated = FRotationMatrix(input_rotate).GetUnitAxis(EAxis::X);
		const FRotator rotate_calc = UKismetMathLibrary::RInterpTo(m_character->GetActorRotation(), direction_rotated.Rotation(), Delta, 10);

		m_character->SetActorRotation(rotate_calc.Quaternion());
	} 

	// Rotate Spine	-------------------------------------------
	const FRotator player_rotator = UKismetMathLibrary::MakeRotFromX(m_character->GetActorForwardVector());
	const FRotator target_rotator = UKismetMathLibrary::FindLookAtRotation(m_character->GetActorLocation(), Target->GetActorLocation());
	m_spine_rotation = target_rotator - player_rotator;

	if (20.f > FMath::Abs(m_spine_rotation.Yaw) || 45.f < FMath::Abs(m_spine_rotation.Yaw)) m_spine_rotation = FRotator::ZeroRotator;
}

void UC0000_AnimInstance::CheckNotInput(const FVector& last_input_vector)
{
	if (last_input_vector.Size() > 0) 
	{
		m_time_cur_input = m_time_delta;
		m_is_stop_move	 = false;
	}
	else if ((m_time_delta > m_time_cur_input + 0.1) && (EGroundState::EGS_Walk < m_character->GetGroundState()))
	{
		EActionState action = m_character->GetActionState();
		if (EActionState::EAS_Dodgeing == action ||
			EActionState::EAS_Guarding == action) return;

		m_time_cur_input = m_time_delta;
		m_is_stop_move	 = true;

		m_character->SetGroundUnoccupied();
	}
}

void UC0000_AnimInstance::CheckEnableSprintTurn()
{
	if (m_character->GetGroundState() != EGroundState::EGS_Sprint)
	{ 
		ResetEnableSprintTurn();
	}
	else if (m_time_delta > m_time_cur_sprint_turn + 0.5)
	{
		m_enable_sprint_turn = true;
	}
}

void UC0000_AnimInstance::SetRotateAttackByInput(const float& Delta)
{
	m_only_move_side = false;

	if (m_character->GetInputEnableAttackRotate() == false ||
		m_character->GetActionState() != EActionState::EAS_Attacking) return;

	float	axis_x		= 0;
	float	axis_y		= 0;
	bool	move_axis_x = m_character->IsInputKey(FName("MoveForward"), axis_x, true);
	bool	move_axis_y = m_character->IsInputKey(FName("MoveSide"), axis_y, true);

	if (false == move_axis_x && false == move_axis_y) return;

	const FRotator control_rotation	= m_character->GetControlRotation();

	const FRotator Yaw_rotation(0.f, control_rotation.Yaw, 0.f);
	const FVector  direction_x  = FRotationMatrix(Yaw_rotation).GetUnitAxis(EAxis::X) * axis_x;
	const FVector  direction_y  = FRotationMatrix(Yaw_rotation).GetUnitAxis(EAxis::Y) * axis_y;
	const FRotator rotate_calc	= UKismetMathLibrary::RInterpTo(m_character->GetActorRotation(), (direction_x + direction_y).Rotation(), Delta, 10);

	m_character->SetActorRotation(rotate_calc.Quaternion());
}

void UC0000_AnimInstance::SetRotateAttack(const float& Delta)
{
	AActor* target = m_character->GetTarget();
	if (!target) return;

	const FVector  dir = target->GetActorLocation() - m_character->GetActorLocation();
	const FRotator rotate_calc = UKismetMathLibrary::RInterpTo(m_character->GetActorRotation(), dir.Rotation(), Delta, 10);

	m_character->SetActorRotation(rotate_calc.Quaternion());
}

bool UC0000_AnimInstance::IsTryingMove()
{
	float scale;
	if (m_character->IsInputKey(FName("MoveForward"), scale, true)) { return true; }
	if (m_character->IsInputKey(FName("MoveSide"), scale, true))	 { return true; }

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

void UC0000_AnimInstance::AnimNotify_MontageJumpSection()
{
	if (!m_character || !m_request_montage) return;
	EActionState state = m_character->GetActionState();

	switch (state)
	{
		case EActionState::EAS_Sit:
		{
			Montage_Play(m_request_montage);
			Montage_JumpToSection(FName("SitLoop"), m_request_montage);
			
			FOnMontageEnded callback;
			callback.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)->void { m_request_montage = nullptr; });
			Montage_SetEndDelegate(callback);
		}
		break;
	}
}

void UC0000_AnimInstance::AnimNotify_EnableBoxCollision()
{
	if (nullptr == m_character) return;
	m_character->SetWeaponCollision(ECollisionEnabled::Type::QueryOnly);
}

void UC0000_AnimInstance::AnimNotify_DisableBoxCollision()
{
	if (nullptr == m_character) return;
	m_character->SetWeaponCollision(ECollisionEnabled::Type::NoCollision);
}

void UC0000_AnimInstance::AnimNotify_JumpStart()
{
	if (nullptr == m_movement_component || nullptr == m_character) return;

	m_movement_component->SetMovementMode(EMovementMode::MOVE_Flying);
	m_movement_component->bNotifyApex = true;

	m_character->SetJumpStartLocation(m_character->GetActorLocation());
}

void UC0000_AnimInstance::AnimNotify_OnLandEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetLandingFinish();
}

void UC0000_AnimInstance::AnimNotify_OnDodgeEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetActionUnoccupied();

	if (m_character->IsInputAxisActionKey())
	{
		m_character->OnDodgeEnd();
	}
}

void UC0000_AnimInstance::AnimNotify_OnTurnEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnEndTurn();
}

void UC0000_AnimInstance::AnimNotify_OnApex()
{
	if (nullptr == m_movement_component) return;

	m_character->GetJumpDirectionLength(m_character->GetActorLocation());

	SetRootMotionMode(ERootMotionMode::NoRootMotionExtraction);
	m_movement_component->SetMovementMode(EMovementMode::MOVE_Falling);

	m_movement_component->bNotifyApex = false;
	m_movement_component->NotifyJumpApex();
}

void UC0000_AnimInstance::AnimNotify_Swap()
{
	CHECK_INVALID_PTR(m_character)
	EWeaponEquipHand hand_type = m_character->GetSwapHand();

	m_character->Swap_Weapon(hand_type);
}

void UC0000_AnimInstance::AnimNotify_SwapEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetActionUnoccupied();
}

void UC0000_AnimInstance::AnimNotify_ReserveNextAttack()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetAttackEnable(true);
}

void UC0000_AnimInstance::AnimNotify_OnAttackEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnEndAttack();

}

void UC0000_AnimInstance::AnimNotify_OnEnableInput()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetInputMovementEnable(true);
	m_character->SetAttackEnable(true);
}

void UC0000_AnimInstance::AnimNotify_OnDisableInput()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetInputMovementEnable(false);
	m_character->SetInputAttackRotateEnable(false);

	m_time_cur_input = m_time_delta;
	m_is_stop_move	 = false;
}

void UC0000_AnimInstance::AnimNotify_OnMoveStopStart()
{

}

void UC0000_AnimInstance::AnimNotify_OnMoveStopEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_is_stop_move = false;
}

void UC0000_AnimInstance::AnimNotify_OnGuardStart()
{
	m_is_guard = true;
	UE_LOG(LogTemp, Warning, TEXT("Guard Start"))
}

void UC0000_AnimInstance::AnimNotify_OnGuardEnd()
{
	m_is_guard = false;
	CHECK_INVALID_PTR(m_character)

	if (false == m_character->IsInputAxisActionKey())
	{
		m_character->OnGuardEnd();
	}
}

void UC0000_AnimInstance::AnimNotify_OnKnockbackStart()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnStartKnockback();

	m_time_cur_input = m_time_delta;
	m_is_stop_move	 = false;
}

void UC0000_AnimInstance::AnimNotify_OnKnockbackEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnEndKnockback();
}

void UC0000_AnimInstance::AnimNotify_OnEnableParryCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetParryCollision(ECollisionEnabled::QueryOnly);
}

void UC0000_AnimInstance::AnimNotify_OnDisableParryCollision()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetParryCollision(ECollisionEnabled::NoCollision);
}

void UC0000_AnimInstance::AnimNotify_OnParryStart()
{
	CHECK_INVALID_PTR(m_character)

	m_character->SetInputMovementEnable(false);
	m_character->SetAttackEnable(false);

	m_time_cur_input = m_time_delta;
	m_is_stop_move	 = false;
}

void UC0000_AnimInstance::AnimNotify_OnParryEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnEndAttack();
	m_character->SetInputMovementEnable(true);
	m_character->SetAttackEnable(true);

	m_time_cur_input = m_time_delta;
	m_is_stop_move	 = false;
}

void UC0000_AnimInstance::AnimNotify_OnCameraShakeIn()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnCameraShakeIn();
}

void UC0000_AnimInstance::AnimNotify_OnCameraShakeOut()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnCameraShakeOut();
}

void UC0000_AnimInstance::AnimNotify_OnEnableShortAttack()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetShortAttackEnable(true);
}

void UC0000_AnimInstance::AnimNotify_OnDisableShortAttack()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetShortAttackEnable(false);
}

void UC0000_AnimInstance::AnimNotify_OnSuccessConsume()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnSuccessConsume();
}

void UC0000_AnimInstance::AnimNotify_OnEndConsume()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnEndConsume();
}

void UC0000_AnimInstance::AnimNotify_OnReactEnd()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnReactEnd();
}

void UC0000_AnimInstance::AnimNotify_OnUseStamina()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnUseStamina(50);
}

void UC0000_AnimInstance::AnimNotify_OnUseStaminaShort()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnUseStamina(30);
}

void UC0000_AnimInstance::AnimNotify_OnHideWeapon()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnHideWeapon(true);
}

void UC0000_AnimInstance::AnimNotify_OnShowWeapon()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnShowWeapon(true);
}

void UC0000_AnimInstance::AnimNotify_EnableDamageMod()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetDamageMod(true);
}

void UC0000_AnimInstance::AnimNotify_DisableDamageMod()
{
	CHECK_INVALID_PTR(m_character)
	m_character->SetDamageMod(false);
}

void UC0000_AnimInstance::AnimNotify_OnEndExecution()
{
	CHECK_INVALID_PTR(m_character)
	m_character->OnEndDirection();
	m_character->OnEndAttack();
	m_character->OnEndExecution();
}

void UC0000_AnimInstance::AnimNotify_OnEndEnterToBoss()
{
	CHECK_INVALID_PTR(m_character)
	APlayerController* controller = m_character->GetControllerComponent();
	CHECK_INVALID_PTR(controller)
	m_character->EnableInput(controller);
}
