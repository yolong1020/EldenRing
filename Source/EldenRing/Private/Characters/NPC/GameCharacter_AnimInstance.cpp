// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/GameCharacter_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/GameCharacter.h"
#include "Characters/NPC/NPC_Character.h"
#include "FieldObject/AssemblyPoint/AssemblyPoint.h"
#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "TimerManager.h"
#include "Macro/DebugMacros.h"

void UGameCharacter_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_npc = Cast<ANPC_Character>(TryGetPawnOwner());
	if (!m_npc) return;

	m_npc_movement		 = m_npc->GetCharacterMovement();
	m_is_blend_per_bone  = false;
	m_hit_cycle_type	 = EHitCycleType::EHCT_None;
}

void UGameCharacter_AnimInstance::NativeUpdateAnimation(float delta)
{
	Super::NativeUpdateAnimation(delta);

	if (m_npc_movement)
	{
		m_ground_speed		= UKismetMathLibrary::VSizeXY(m_npc_movement->Velocity);
		m_is_falling		= m_npc_movement->IsFalling();
		m_is_reached_apex	= m_npc_movement->bNotifyApex;
	}
	if (m_npc)
	{
		m_action_state		= (EActionState_NPC)m_npc->GetActionState();
		m_equip_state		= (EEquipState)m_npc->GetWeaponEquipState();
		m_vigilance_state	= m_npc->GetVigilanceState();
		m_death_pose		= m_npc->GetDeathPose();
		m_battle_pose		= m_npc->GetBattlePose();

		RestTransformFix(delta);
	}
}

void UGameCharacter_AnimInstance::SetHitReactProperties(const bool& is_need_blend, const int32& animation_index, const EAttackWeight& attack_weight, const EHitCycleType& cycle_type)
{
	m_is_blend_per_bone		= is_need_blend;
	m_hit_animation_index	= animation_index;
	m_hit_attack_weight		= attack_weight;
	m_hit_cycle_type		= cycle_type;
}

void UGameCharacter_AnimInstance::RestTransformFix(const float& delta)
{
	CHECK_INVALID_PTR(m_npc)

	AAssemblyPoint*			assembly_point	= m_npc->GetAssemblyPoint();
	AAssemblePointObject*	point_object	= m_npc->GetAssemblyPointObject();

	if (!assembly_point || !point_object) return;
	m_rest_state = (nullptr == point_object) ? EAssemblyPointObjectType::EAPOT_None : point_object->GetPointObjectType();

	if (EActionState_NPC::EASN_Resting == m_action_state)
	{
		FVector actor_location			 = m_npc->GetActorLocation();
		FVector assemble_location		 = assembly_point->GetActorLocation();
		FVector assemble_offset_location = FVector(assemble_location.X, assemble_location.Y, actor_location.Z);
		FVector point_object_location	 = point_object->GetActorLocation();
		FVector point_offset_location	 = FVector(point_object_location.X, point_object_location.Y, actor_location.Z);

		FVector dir = (assemble_offset_location - point_object_location).GetSafeNormal2D();

		FVector	 location_calc	= UKismetMathLibrary::VInterpTo(actor_location, point_offset_location, delta, 2);
		FRotator rotate_calc	= UKismetMathLibrary::RInterpTo(m_npc->GetActorRotation(), dir.Rotation(), delta, 10);

		m_npc->SetActorLocationAndRotation(location_calc, rotate_calc);
	}
}

void UGameCharacter_AnimInstance::AnimNotify_MontageStart()
{
	if (RootMotionMode == ERootMotionMode::RootMotionFromEverything)
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);

		CHECK_INVALID_PTR(m_npc_movement)
		m_npc_movement->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}
}

void UGameCharacter_AnimInstance::AnimNotify_MontageEnd()
{
	if (RootMotionMode == ERootMotionMode::RootMotionFromMontagesOnly)
	{
		SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		
		CHECK_INVALID_PTR(m_npc_movement)
		m_npc_movement->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}
}

void UGameCharacter_AnimInstance::AnimNotify_EnableBoxCollision()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->SetWeaponCollision(ECollisionEnabled::QueryOnly);
}

void UGameCharacter_AnimInstance::AnimNotify_DisableBoxCollision()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->SetWeaponCollision(ECollisionEnabled::NoCollision);
}

void UGameCharacter_AnimInstance::AnimNotify_OnVigilance()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnChangeVigilanceState(EVigilanceState::EVS_Vigilance);
}

void UGameCharacter_AnimInstance::AnimNotify_OnRepose()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnChangeVigilanceState(EVigilanceState::EVS_Repose);
}

void UGameCharacter_AnimInstance::AnimNotify_AfterDeath()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnDeathCompleted();
}

void UGameCharacter_AnimInstance::AnimNotify_OnNextAttack()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnNextAttack();
}

void UGameCharacter_AnimInstance::AnimNotify_OnEndAttack()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnEndAttack();
}

void UGameCharacter_AnimInstance::AnimNotify_OnReactEnd()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnReactEnd();
}

void UGameCharacter_AnimInstance::AnimNotify_OnRotateEnd()
{
	CHECK_INVALID_PTR(m_npc)

	SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

	EActionState_NPC state = (EActionState_NPC)m_npc->GetActionState();
	if (state == EActionState_NPC::EASN_ReturningPointObject)
	{
		m_npc->OnTurnEndForAssembly();
	}
	else
	{
		m_npc->OnTurnEnd();
	}
}

void UGameCharacter_AnimInstance::AnimNotify_OnEndStunn()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnEndStunn();
}

void UGameCharacter_AnimInstance::AnimNotify_OnTakeExecutionEnd()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnTakeExecutionEnd();
	m_npc->OnReactEnd();
}

void UGameCharacter_AnimInstance::AnimNotify_OnWatchingToIdle()
{
	CHECK_INVALID_PTR(m_npc)

	bool is_idle_to_watching = m_npc->GetWatchingSwitch();
	if (is_idle_to_watching) { return; }
	
	m_npc->OnWatchingSwitch();
}

void UGameCharacter_AnimInstance::AnimNotify_OnIdleToWatching()
{
	CHECK_INVALID_PTR(m_npc)

	bool is_idle_to_watching = m_npc->GetWatchingSwitch();
	if (false == is_idle_to_watching) { return; }

	m_npc->OnWatchingSwitch();
}

void UGameCharacter_AnimInstance::AnimNotify_OnRestingToIdle()
{
	CHECK_INVALID_PTR(m_npc)

	bool is_idle_to_resting = m_npc->GetRestingSwitch();
	if (is_idle_to_resting) return;

	m_npc->OnRestingSwitch();
}

void UGameCharacter_AnimInstance::AnimNotify_OnIdleToResting()
{
	CHECK_INVALID_PTR(m_npc)

	bool is_idle_to_resting = m_npc->GetRestingSwitch();
	if (false == is_idle_to_resting) return;

	m_npc->OnRestingSwitch();
}

void UGameCharacter_AnimInstance::AnimNotify_OnSwapWeaponR()
{
	CHECK_INVALID_PTR(m_npc)
	m_npc->OnSwapWeaponR();
}

void UGameCharacter_AnimInstance::AnimNotify_OnChangeDeathFront()
{
	CHECK_INVALID_PTR(m_npc)
	m_is_death_by_execution = m_npc->IsNeedChangeDeath(EGameDirection::EGD_Front);
}

void UGameCharacter_AnimInstance::AnimNotify_OnChangeDeathBack()
{
	CHECK_INVALID_PTR(m_npc)
	m_is_death_by_execution = m_npc->IsNeedChangeDeath(EGameDirection::EGD_Back);
}