// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/GameCharacter_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/GameCharacter.h"
#include "Characters/NPC/NPC_Character.h"
#include "FieldObject/AssemblyPoint/AssemblyPoint.h"
#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "TimerManager.h"
#include "Basic/DebugMacros.h"

void UGameCharacter_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_character_game = Cast<ANPC_Character>(TryGetPawnOwner());
	CHECK_INVALID(m_character_game)

	m_character_movement 	= m_character_game->GetCharacterMovement();
	m_is_blend_per_bone 	= false;
	m_hit_cycle_type	= EHitCycleType::EHCT_None;
}

void UGameCharacter_AnimInstance::NativeUpdateAnimation(float delta)
{
	Super::NativeUpdateAnimation(delta);

	CHECK_INVALID(m_character_movement)
	m_ground_speed		= UKismetMathLibrary::VSizeXY(m_character_movement->Velocity);
	m_is_falling		= m_character_movement->IsFalling();
	m_is_reached_apex	= m_character_movement->bNotifyApex;

	CHECK_INVALID(m_character_game)
	m_action_state		= m_character_game->GetActionState();
	m_equip_state		= m_character_game->GetEquipState();
	m_vigilance_state	= m_character_game->GetVigilanceState();
	m_death_pose		= m_character_game->GetDeathPose();
	m_battle_pose		= m_character_game->GetBattlePose();

	AAssemblyPoint*		assembly_point	= m_character_game->GetAssemblyPoint();
	AAssemblePointObject*	point_object	= m_character_game->GetAssemblyPointObject();

	m_rest_state = (nullptr == point_object) ? EAssemblyPointObjectType::EAPOT_None : point_object->GetPointObjectType();
	if (nullptr == assembly_point || nullptr == point_object) return;

	if (EActionState_NPC::EASN_Resting == m_action_state)
	{
		FVector actor_location			= m_character_game->GetActorLocation();
		FVector assemble_location		= assembly_point->GetActorLocation();
		FVector assemble_offset_location	= FVector(assemble_location.X, assemble_location.Y, actor_location.Z);
		FVector point_object_location		= point_object->GetActorLocation();
		FVector point_offset_location		= FVector(point_object_location.X, point_object_location.Y, actor_location.Z);

		FVector  dir 		= (assemble_offset_location - actor_location).GetSafeNormal2D();
		FVector	 location_calc	= UKismetMathLibrary::VInterpTo(actor_location, point_offset_location, delta, 2);
		FRotator rotate_calc	= UKismetMathLibrary::RInterpTo(m_character_game->GetActorRotation(), dir.Rotation(), delta, 10);

		m_character_game->SetActorLocationAndRotation(location_calc, rotate_calc);
	}
}

void UGameCharacter_AnimInstance::SetHitReactProperties(const bool& is_need_blend, const int32& animation_index, const EAttackWeight& attack_weight, const EHitCycleType& cycle_type)
{
	m_is_blend_per_bone	= is_need_blend;
	m_hit_animation_index	= animation_index;
	m_hit_attack_weight	= attack_weight;
	m_hit_cycle_type	= cycle_type;
}

void UGameCharacter_AnimInstance::AnimNotify_MontageStart()
{
	if (RootMotionMode == ERootMotionMode::RootMotionFromEverything)
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);

		CHECK_INVALID(m_character_movement)
		m_character_movement->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}
}

void UGameCharacter_AnimInstance::AnimNotify_MontageEnd()
{
	if (RootMotionMode == ERootMotionMode::RootMotionFromMontagesOnly)
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		
		CHECK_INVALID(m_character_movement)
		m_character_movement->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}
}

void UGameCharacter_AnimInstance::AnimNotify_EnableBoxCollision()
{
	CHECK_INVALID(m_character_game)
	m_character_game->SetWeaponCollision(ECollisionEnabled::QueryOnly);
}

void UGameCharacter_AnimInstance::AnimNotify_DisableBoxCollision()
{
	CHECK_INVALID(m_character_game)
	m_character_game->SetWeaponCollision(ECollisionEnabled::NoCollision);
}

void UGameCharacter_AnimInstance::AnimNotify_OnVigilance()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnChangeVigilanceState(EVigilanceState::EVS_Vigilance);
}

void UGameCharacter_AnimInstance::AnimNotify_OnRepose()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnChangeVigilanceState(EVigilanceState::EVS_Repose);
}

void UGameCharacter_AnimInstance::AnimNotify_AfterDeath()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnDeathCompleted();
}

void UGameCharacter_AnimInstance::AnimNotify_OnNextAttack()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnNextAttack();
}

void UGameCharacter_AnimInstance::AnimNotify_OnEndAttack()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnEndAttack();
}

void UGameCharacter_AnimInstance::AnimNotify_OnReactEnd()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnReactEnd();
}

void UGameCharacter_AnimInstance::AnimNotify_OnRotateEnd()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnTurnEnd();
}

void UGameCharacter_AnimInstance::AnimNotify_OnEndStunn()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnEndStunn();
}

void UGameCharacter_AnimInstance::AnimNotify_OnTakeExecutionEnd()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnTakeExecutionEnd();
	m_character_game->OnReactEnd();
}

void UGameCharacter_AnimInstance::AnimNotify_OnWatchingToIdle()
{
	CHECK_INVALID(m_character_game)

	bool is_idle_to_watching = m_character_game->GetWatchingSwitch();
	if (is_idle_to_watching) return;
	
	m_character_game->OnWatchingSwitch();
}

void UGameCharacter_AnimInstance::AnimNotify_OnIdleToWatching()
{
	CHECK_INVALID(m_character_game)

	bool is_idle_to_watching = m_character_game->GetWatchingSwitch();
	if (false == is_idle_to_watching) return;

	m_character_game->OnWatchingSwitch();
}

void UGameCharacter_AnimInstance::AnimNotify_OnRestingToIdle()
{
	CHECK_INVALID(m_character_game)

	bool is_idle_to_resting = m_character_game->GetRestingSwitch();
	if (is_idle_to_resting) return;

	m_character_game->OnRestingSwitch();
}

void UGameCharacter_AnimInstance::AnimNotify_OnIdleToResting()
{
	CHECK_INVALID(m_character_game)

	bool is_idle_to_resting = m_character_game->GetRestingSwitch();
	if (false == is_idle_to_resting) return;

	m_character_game->OnRestingSwitch();
}

void UGameCharacter_AnimInstance::AnimNotify_OnSwapWeaponR()
{
	CHECK_INVALID(m_character_game)
	m_character_game->OnSwapWeaponR();
}

void UGameCharacter_AnimInstance::AnimNotify_OnChangeDeathFront()
{
	CHECK_INVALID(m_character_game)
	m_is_death_by_execution = m_character_game->IsNeedChangeDeath(EGameDirection::EGD_Front);
}

void UGameCharacter_AnimInstance::AnimNotify_OnChangeDeathBack()
{
	CHECK_INVALID(m_character_game)
	m_is_death_by_execution = m_character_game->IsNeedChangeDeath(EGameDirection::EGD_Back);
}
