// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C4311.h"
#include "Characters/NPC/GameCharacter_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "HUD/Status/HealthBarComponent.h"
#include "Items/Weapons/MeleeAttack_Actor.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Items/Weapons/Shield_Actor.h"
#include "FieldObject/AssemblyPoint/AssemblyPoint.h"
#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "Animation/AnimLinkableElement.h"
#include "TimerManager.h"
#include "Basic/DebugMacros.h"

AC4311::AC4311()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AC4311::BeginPlay()
{
	Super::BeginPlay();

	if (EquipWeapon("Weapon_SteathSocket", EWeaponEquipHand::EWEH_Right))
	{
		m_equip_state = EEquipState::EES_EquippedOneHandedWeapon;

		if (nullptr != m_class_weapon_L)
		{
			EquipWeapon("L_HandSocket", EWeaponEquipHand::EWEH_Left);
		}
	}

	if (m_action_state == EActionState_NPC::EASN_Patrolling)
	{
		CHECK_INVALID(m_target_patrol)
		MoveToTarget(m_target_patrol);

		m_is_patrolling = true;
		m_is_resting	= false;

		TDelegate patrol_delegate = FTimerDelegate::CreateLambda([this]()->void {
			m_is_resting = true;
			//OnMoveOtherPatrolPoint();
			GetWorldTimerManager().ClearTimer(m_timer_patrol);
			});

		GetWorldTimerManager().SetTimer(
			m_timer_patrol,
			patrol_delegate,
			m_time_patrol,
			false);
	}

	CHECK_INVALID(m_assembly_point)
	m_assembly_point->RegisterAssemblyPointMember(this);
}

void AC4311::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC4311::GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight)
{
	CHECK_INVALID(m_attribute)

	if (m_attribute->IsAlive())
	{
		EGameDirection hit_direction = FindDirection(this, ImpactPoint);

		UCharacterMovementComponent* movement_component = GetCharacterMovement();
		CHECK_INVALID(movement_component)
		movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;

		StopAllMontage();
		HitReact(hit_direction, attack_weight);

		if (m_vigilance_state != EVigilanceState::EVS_Vigilance && FName("R_HandSocket") == m_equiped_weapon_R->GetAttachSocketName())
		{
			m_vigilance_state = EVigilanceState::EVS_Vigilance; 
		}

		CHECK_INVALID(m_widget_healthbar)
		m_widget_healthbar->SetVisibility(true);
	}
	else
	{
		if (EDeathPose::EDP_Alive != m_death_pose) { return; }

		UGameCharacter_AnimInstance* anim_instance = Cast<UGameCharacter_AnimInstance>(GetMesh()->GetAnimInstance());
		CHECK_INVALID(anim_instance)
		anim_instance->SetHitReactProperties(false, 0, EAttackWeight::EAW_None, EHitCycleType::EHCT_None);

		FName section_name;
		switch (FMath::RandRange(0, m_montage_death->GetNumSections() - 1))
		{
		case 0: section_name = FName("Death_Front"); m_death_pose = EDeathPose::EDP_Death_Front; break;
		case 1: section_name = FName("Death_Back");  m_death_pose = EDeathPose::EDP_Death_Back; break;
		}

		SetBlindPawn(true);
		ClearAllTimer();

		PlayMontage(m_montage_death, section_name);
		m_action_state = EActionState_NPC::EASN_Unoccupied;

		CHECK_INVALID(m_widget_healthbar)
		m_widget_healthbar->SetVisibility(false);
	}

	if (nullptr != m_sound_hit) { UGameplayStatics::PlaySoundAtLocation(this, m_sound_hit, ImpactPoint); }

	if (nullptr != m_particle_hit)
	{
		auto world = GetWorld();
		CHECK_INVALID(world)

		UGameplayStatics::SpawnEmitterAtLocation(world, m_particle_hit, ImpactPoint);			
	}

#ifdef _DEBUG
	DRAW_SPHERE_DURATION(ImpactPoint)
#endif
}

void AC4311::SetWeaponCollision(ECollisionEnabled::Type type)
{
	CHECK_INVALID(m_equiped_weapon_R)

	AMeleeAttack_Actor* weapon = Cast<AMeleeAttack_Actor>(m_equiped_weapon_R);
	CHECK_INVALID(weapon)

	auto collision_box = weapon->GetWeaponBox();
	if (nullptr != collision_box)
	{
		collision_box->SetCollisionEnabled(type);
	}
	m_equiped_weapon_R->m_ignore_actors.Empty();
}

float AC4311::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (EVigilanceState::EVS_Vigilance == m_vigilance_state)
	{
		// 타겟 변경 조건 필요
		// m_actor_target = EventInstigator->GetPawn();


	}
	else
	{
		m_actor_target = Cast<AGameCharacter>(EventInstigator->GetPawn());
	}

	return DamageAmount;
}

void AC4311::HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight)
{
	UGameCharacter_AnimInstance* anim_instance = Cast<UGameCharacter_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID(anim_instance)

	bool			is_need_blend			= true;
	FString			hit_section_postfix		= "";
	int32			selected_index			= 0;

	UAnimMontage*	montage_with_direction	= nullptr;
	UAnimMontage*	montage_no_direction	= nullptr;

	switch (attack_weight)
	{
	case EAttackWeight::EAW_Small:
	{
		hit_section_postfix		= FString("Small");
	}break;
	case EAttackWeight::EAW_Medium:
	{
		hit_section_postfix		= FString("Medium");
		montage_no_direction	= m_montage_hit_medium;
		montage_with_direction	= m_montage_hit_medium_direction;
	} break;
	case EAttackWeight::EAW_Heavy:
	case EAttackWeight::EAW_ExtraHeavy:
	case EAttackWeight::EAW_UltraHeavy: 
	{
		hit_section_postfix		= FString("UltraHeavy");
		montage_with_direction	= nullptr;

		PlayMontage(m_montage_hit_ultra_heavy_directiony, FName(hit_section_postfix));
		anim_instance->SetHitReactProperties(is_need_blend, selected_index, attack_weight, EHitCycleType::EHCT_Even);
		anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	}
	}

	if (hit_section_postfix != FString("UltraHeavy"))
	{
		CHECK_INVALID(montage_no_direction)
		CHECK_INVALID(montage_with_direction)

		selected_index = FMath::RandRange(0, montage_no_direction->GetNumSections() - 1);
		switch (hit_direction)
		{
		case EGameDirection::EGD_Back:	hit_section_postfix	+= FString("_Back");  break;
		case EGameDirection::EGD_Left:	hit_section_postfix	+= FString("_Left");  break;
		case EGameDirection::EGD_Right:	hit_section_postfix	+= FString("_Right"); break;
		case EGameDirection::EGD_Front: 
		{
			is_need_blend		= false;
			selected_index		= FMath::RandRange(1, montage_no_direction->GetNumSections());
			hit_section_postfix += (FString("_0") + FString::FromInt(selected_index));
		} break;
		default: { return; }
		}

		EHitCycleType cycle_type = anim_instance->GetHitCycle();
		anim_instance->SetHitReactProperties(is_need_blend, selected_index, attack_weight, cycle_type == EHitCycleType::EHCT_Even ? EHitCycleType::EHCT_Odd : EHitCycleType::EHCT_Even);
		anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);

		if (hit_direction != EGameDirection::EGD_Front) { PlayMontage(montage_with_direction, FName(hit_section_postfix)); }
		else											{ PlayMontage(montage_no_direction, FName(hit_section_postfix)); }
		
		bool is_already_clear = false;
		for (FAnimMontageInstance* instance : anim_instance->MontageInstances)
		{
			if (false	== instance->bPlaying	||
				nullptr == instance->Montage	||
				false	== instance->Montage->GetFName().ToString().Contains("AM_HitReact")) { continue; }

			int32 index = instance->Montage->GetSectionIndex(FName(hit_section_postfix));
			if (0 > index) { continue; }

			FCompositeSection section		= instance->Montage->GetAnimCompositeSection(index);
			int32			  segment_index = section.GetSegmentIndex();

			if (false == instance->Montage->SlotAnimTracks.IsValidIndex(0)) { continue; }

			FSlotAnimationTrack& default_slot = instance->Montage->SlotAnimTracks[0];
			if (false == default_slot.AnimTrack.AnimSegments.IsValidIndex(segment_index)) { continue; }

			FAnimSegment segment = default_slot.AnimTrack.AnimSegments[segment_index];
			float		 length	 = segment.GetLength();

			bool is_need_clear = (hit_direction == EGameDirection::EGD_Front) ? (instance->Montage == montage_no_direction) : (instance->Montage == montage_with_direction);
			if (false == is_already_clear && is_need_clear)
			{
				GetWorldTimerManager().ClearTimer(m_timer_hit);
				GetWorldTimerManager().SetTimer(m_timer_hit, this, &AC4311::OnReactEnd, length);
				is_already_clear = true;
			}
		}
	}

	m_action_state = EActionState_NPC::EASN_HitReact;
}

void AC4311::PawnSeen(APawn* seen_pawn)
{
	/* if (세력 검사) return; */

	AGameCharacter* target = Cast<AGameCharacter>(seen_pawn);
	if (nullptr == target || IsCantMoveState()) { return; }

	if (nullptr == m_actor_target)
	{
		if (seen_pawn->ActorHasTag(FName("Player")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("PawnSeen : Start Combat"));
			StartCombat();
			m_actor_target = target;
		}
	}
	else if (target == m_actor_target && EVigilanceState::EVS_Vigilance == m_vigilance_state)
	{
		OnCheckMissingTarget();

		// 범위 내
		if (InTargetRange(m_actor_target, m_radius_tracking))
		{
			if (InTargetRange(m_actor_target, m_radius_confront) == false)
			{
				if (m_action_state != EActionState_NPC::EASN_Attacking &&
					m_action_state != EActionState_NPC::EASN_TakeExecution)
				{
					UE_LOG(LogTemp, Warning, TEXT("Is Tracking"));

					ChangeRootMotionMode(ERootMotionMode::RootMotionFromEverything);
					MoveToTarget(m_actor_target, m_radius_attack * 0.5);
				}

				m_action_state = EActionState_NPC::EASN_Chasing;
				if (nullptr != Cast<AShield_Actor>(m_equiped_weapon_L)) { m_battle_pose = EBattlePose::EBP_Defense; }

				GetWorldTimerManager().ClearTimer(m_timer_reserve_action);
			}
		}
		// 범위 이탈
		else
		{
			FString state = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));
			//UE_LOG(LogTemp, Warning, TEXT("Out Tracking PawnSeen State : %s"), *state);

			if (m_action_state != EActionState_NPC::EASN_Attacking &&
				m_action_state != EActionState_NPC::EASN_TakeExecution)
			{
				FinishVigilance();
			}
		}
	}
}

void AC4311::StartWatching()
{
	Super::StartWatching();

	if (40.f >= FMath::Rand() % 100)
	{
		PlayMontage(m_montage_watching, FName("Watching_Switch"), -1);
		m_is_idle_to_watching = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On Move Other Patrol Point by StartWatching"))

		const float wait_sec = FMath::RandRange(m_time_wait_min, m_time_wait_max);
		GetWorldTimerManager().SetTimer(m_timer_watching, this, &AC4311::OnMoveOtherPatrolPoint, wait_sec);
	}
}

void AC4311::StartCombat()
{
	if (m_vigilance_state == EVigilanceState::EVS_Vigilance	|| IsCantMoveState()) return;

	GetWorldTimerManager().ClearTimer(m_timer_watching);
	GetWorldTimerManager().ClearTimer(m_timer_patrol);

	PlayMontage(m_montage_swap, FName("Swap_Battle"));

	m_action_state = EActionState_NPC::EASN_Swap;
}

void AC4311::StartAttack()
{
	FString status = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));

	m_action_state		= EActionState_NPC::EASN_Attacking;
	m_battle_pose		= EBattlePose::EBP_Attack;
	m_attack_strength	= ((FMath::Rand() % 100) > 50) ? EAttackStrength::EATKS_Normal : EAttackStrength::EATKS_Strong;
	m_attack_type_prev	= EAttackType::EATKT_Attack;

	FString section_name = "";
	if (m_attack_strength != EAttackStrength::EATKS_Normal) { 
		if ((FMath::Rand() % 100) > 50) { section_name = FString("Strong_Attack_"); }
		else							{ section_name = FString("Dash_Attack"); }
	}
	else { section_name = FString("Attack_"); }

	if (section_name != FString("Dash_Attack")) {
		if ((FMath::Rand() % 100) > 50) { section_name += FString("01"); }
		else							{ section_name += FString("02"); }
	}
	m_equiped_weapon_R->SetAttackWeight(m_equip_state, m_attack_strength, m_attack_type_prev);

	//UE_LOG(LogTemp, Warning, TEXT("Play Montage : Start Attack"));
	PlayMontage(m_montage_attack, FName(section_name));

	m_attack_name_prev = FName(section_name);
}

void AC4311::StartTurn(const EGameDirection& direction)
{
	if (nullptr != Cast<AShield_Actor>(m_equiped_weapon_L)) { m_battle_pose = EBattlePose::EBP_Defense; }

	FString pre_fix = FString();
	switch (m_battle_pose)
	{
	case EBattlePose::EBP_Attack:	pre_fix = FString("Battle_"); break;
	case EBattlePose::EBP_Defense:	pre_fix = FString("Shield_"); break;
	}

	FString post_fix = FString();
	switch (direction)
	{
	case EGameDirection::EGD_Back:	post_fix = FString("Right_180"); break;
	case EGameDirection::EGD_Left:	post_fix = FString("Left_90"); break;
	case EGameDirection::EGD_Right: post_fix = FString("Right_90"); break;
	}

	FString dir = StaticEnum<EGameDirection>()->GetNameStringByValue(int64(direction));
	UE_LOG(LogTemp, Warning, TEXT("StartTurn : State : %s"), *dir);

	m_action_state = EActionState_NPC::EASN_Turn;

	PlayMontage(m_montage_turn, FName(pre_fix + post_fix));
}

void AC4311::StartConfront()
{
	//UE_LOG(LogTemp, Warning, TEXT("StartConfront"))
	//	2. 경계 자세 - 방어 / 일반
	//		> 일정 시간에 따라서 움직이는 방향을 범위 내 랜덤으로 준다.
	//		> 플레이어 체력에 따라서 공격형 방어형으로 분리한다.
	if (nullptr == m_actor_target) return;

	if (m_action_state == EActionState_NPC::EASN_HitReact) { return; }

	m_action_state	= EActionState_NPC::EASN_Confronting;
	if (nullptr != Cast<AShield_Actor>(m_equiped_weapon_L)) { m_battle_pose = EBattlePose::EBP_Defense; }

	EGameDirection direction = (EGameDirection)FMath::RandRange((int32)EGameDirection::EGD_Back, (int32)EGameDirection::EGD_Right);
	
	bool is_too_close = InTargetRange(m_actor_target, m_radius_attack);
	if (is_too_close) { direction = EGameDirection::EGD_Back; }

	FString section_name;
	switch (direction)
	{
	case EGameDirection::EGD_Back: section_name = FString("ToBack");  break;
	case EGameDirection::EGD_Front:section_name = FString("ToFront"); break;
	case EGameDirection::EGD_Left: section_name	= FString("ToLeft");  break;
	case EGameDirection::EGD_Right:section_name = FString("ToRight"); break;
	}

	if (Cast<AShield_Actor>(m_equiped_weapon_L) != nullptr) { section_name += FString("_Shield"); }

	//UE_LOG(LogTemp, Warning, TEXT("Play Montage : Confront"));
	PlayMontage(m_montage_confront, FName(section_name));

	UAnimInstance* anim_instance = m_mesh->GetAnimInstance();
	if (nullptr != anim_instance)
	{
		anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

		MoveToTarget(m_actor_target);
	}

	float wait_sec = FMath::RandRange(1.f, 2.f);
	GetWorldTimerManager().SetTimer(m_timer_reserve_action, this, &AC4311::ChooseNextAction, wait_sec);
}

void AC4311::OnCheckMissingTarget()
{
	if (IsInSight(m_actor_target))
	{
		//UE_LOG(LogTemp, Warning, TEXT("OnCheckMissingTarget : Catch Target"));
		GetWorldTimerManager().SetTimer(m_timer_sight, this, &AC4311::OnCheckMissingTarget, m_sec_kwon_missing_target);
	}
	else if (InTargetRange(m_actor_target, m_radius_tracking))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCheckMissingTarget : StartTurn"));
		EGameDirection direction = FindDirection(this, m_actor_target->GetActorLocation());

		if (direction != EGameDirection::EGD_Front && false == IsCantMoveState())
		{
			StartTurn(direction);
		}
	}
	else if (false == InTargetRange(m_actor_target, m_radius_tracking))
	{
		//UE_LOG(LogTemp, Warning, TEXT("OnCheckMissingTarget : Missing Target"));
		GetWorldTimerManager().ClearTimer(m_timer_sight);
		FinishVigilance();
	}
}

void AC4311::OnWatchingFinished()
{
	PlayMontage(m_montage_watching, FName("Watching_Switch"));
	m_is_idle_to_watching = false;
}

void AC4311::OnWatchingSwitch()
{
	if (m_is_idle_to_watching)
	{
		m_action_state = EActionState_NPC::EASN_Watching;

		const float wait_sec = FMath::RandRange(m_time_wait_min * 2.f, m_time_wait_max * 2.f);
		GetWorldTimerManager().SetTimer(m_timer_watching, this, &AC4311::OnWatchingFinished, wait_sec);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On Move Other Patrol Point by StartWatching"))
		OnMoveOtherPatrolPoint();
	}
}

void AC4311::OnRestingSwitch()
{
	if (m_is_idle_to_resting)
	{
		m_action_state = EActionState_NPC::EASN_Resting;
	}
	else
	{
		TDelegate patrol_delegate = FTimerDelegate::CreateLambda([this]()->void {
			m_is_resting = true;
			GetWorldTimerManager().ClearTimer(m_timer_patrol);
			});

		GetWorldTimerManager().SetTimer(
			m_timer_patrol,
			patrol_delegate,
			m_time_patrol,
			false);

		UE_LOG(LogTemp, Warning, TEXT("On Move Other Patrol Point by OnRestingSwitch"))
		m_is_patrolling	= true;
		m_target_patrol	= SelectPatrolTarget();
		OnMoveOtherPatrolPoint();
	}
}

void AC4311::OnRestingEnd(const FString& section_name)
{
	UCapsuleComponent* capsule = GetCapsuleComponent();
	CHECK_INVALID(capsule)

	capsule->SetCanEverAffectNavigation(false);

	m_is_idle_to_resting			= false;
	m_is_resting					= false;
	m_target_assembly_object		= nullptr;
	PlayMontage(m_montage_resting, FName(section_name));
}

bool AC4311::InAssmeblyPointAction()
{
	if (false == m_is_resting || nullptr == m_target_assembly_object) { return false; }

	FVector delta			 = m_location_assembly_object;
	FVector action_location  = UKismetMathLibrary::Quat_RotateVector(GetActorRotation().Quaternion(), delta);

	EGameDirection direction = FindDirection(this, m_assembly_point->GetActorLocation());
	m_is_idle_to_resting	 = true;
	m_is_patrolling			 = false;

	if (direction == EGameDirection::EGD_Front)
	{
		UCapsuleComponent* capsule = GetCapsuleComponent();
		if (nullptr == capsule) { return false; }

		capsule->SetCanEverAffectNavigation(true);
		UE_LOG(LogTemp, Warning, TEXT("SetCanEverAffectNavigation"))

		m_action_state = EActionState_NPC::EASN_Resting;
		FVector object_location = FVector(m_location_assembly_object.X, m_location_assembly_object.Y, GetActorLocation().Z);

		FVector assemble_location = m_assembly_point->GetActorLocation();
		FVector assemble_offset_location = FVector(assemble_location.X, assemble_location.Y, GetActorLocation().Z);
		FVector actor_location = GetActorLocation();

		FVector dir = (assemble_offset_location - actor_location).GetSafeNormal2D();

		//UKismetSystemLibrary::DrawDebugArrow(this, actor_location, actor_location + dir * 40.f, 5.f, FColor::Black, 5.f);

		//SetActorLocation(object_location);

		FString name = m_target_assembly_object->GetInteractionSectionName();
		PlayMontage(m_montage_resting, FName(name), -1.f);
	}
	else
	{
		StartTurn(direction);
	}

	return true;
}

FName AC4311::ChooseNextAttack()
{
	if (FName("Attack_01") == m_attack_name_prev)
	{
		m_attack_name_prev	= FName("Attack_03");
		m_attack_type_prev	= EAttackType::EATKT_Attack;
		m_attack_strength	= EAttackStrength::EATKS_Normal;

		return m_attack_name_prev;
	}
	else if (FName("Attack_02") == m_attack_name_prev || FName("Attack_03") == m_attack_name_prev)
	{
		m_attack_name_prev	= (0 == FMath::RandRange(0, 1)) ? FName("Strong_Attack_01") : FName("Strong_Attack_02");
		m_attack_type_prev	= EAttackType::EATKT_Attack;
		m_attack_strength	= EAttackStrength::EATKS_Strong;

		return m_attack_name_prev;
	}

	return FName();
}

void AC4311::ChooseNextAction()
{
	if (m_death_pose != EDeathPose::EDP_Alive || IsCantMoveState()) { return; }

	if (m_action_state != EActionState_NPC::EASN_Chasing)
		m_action_state = ((FMath::Rand() % 100) > 40) ? EActionState_NPC::EASN_Attacking : EActionState_NPC::EASN_Confronting;

	switch (m_action_state)
	{
		case EActionState_NPC::EASN_Attacking:
		{
			StartAttack();
		}
		break;
		case EActionState_NPC::EASN_Confronting:
		{
			if (InTargetRange(m_actor_target, m_radius_tracking))
			{
				if (InTargetRange(m_actor_target, m_radius_confront) == false)
				{
					FString state = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));
					//UE_LOG(LogTemp, Warning, TEXT("ChooseNextAction State : %s"), *state);
					
					if (m_action_state != EActionState_NPC::EASN_Attacking/*&&
						m_action_state != EActionState_NPC::EASN_HitReact &&
						m_action_state != EActionState_NPC::EASN_TakeExecution*/)
					{
						ChangeRootMotionMode(ERootMotionMode::RootMotionFromEverything);
						MoveToTarget(m_actor_target);
					}

					m_action_state = EActionState_NPC::EASN_Chasing;

					GetWorldTimerManager().ClearTimer(m_timer_reserve_action);
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("ChooseNextAction : StartConfront"));
					StartConfront();
				}
			}
			else
			{
				m_action_state = EActionState_NPC::EASN_Unoccupied;

				if (m_action_state != EActionState_NPC::EASN_Attacking &&
					m_action_state != EActionState_NPC::EASN_HitReact &&
					m_action_state != EActionState_NPC::EASN_TakeExecution)
				{
					FinishVigilance();
				}
			}
		}
	}
}

void AC4311::FinishVigilance()
{
	m_action_state = EActionState_NPC::EASN_Swap;
	m_actor_target = nullptr;

	PlayMontage(m_montage_swap, FName("Swap_Repose"));
	StopAllMovement();

	if (nullptr == m_widget_healthbar) return;
	m_widget_healthbar->SetVisibility(false);
}

bool AC4311::IsCantMoveState()
{
	return (m_action_state == EActionState_NPC::EASN_HitReact ||
			m_action_state == EActionState_NPC::EASN_GuardReact ||
			m_action_state == EActionState_NPC::EASN_Swap ||
			m_action_state == EActionState_NPC::EASN_TakeExecution ||
			m_action_state == EActionState_NPC::EASN_Stunning ||
			m_death_pose   != EDeathPose::EDP_Alive);
}

void AC4311::OnChangeVigilanceState(const EVigilanceState& state)
{
	m_action_state = (m_action_state == EActionState_NPC::EASN_Swap) ? EActionState_NPC::EASN_Unoccupied : m_action_state;
	if (IsCantMoveState()) { return; }

	Super::OnChangeVigilanceState(state);

	if (EVigilanceState::EVS_Repose == state)
	{
		m_action_state = EActionState_NPC::EASN_Unoccupied;
		m_battle_pose  = EBattlePose::EBP_Unoccupied;
		StartWatching();
	}
	else if (EVigilanceState::EVS_Vigilance == state)
	{
		// Chase or Shot
		//FString state = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));
		//UE_LOG(LogTemp, Warning, TEXT("OnChangeVigilanceState State : %s"), *state);

		CHECK_INVALID(m_widget_healthbar)
		m_widget_healthbar->SetVisibility(true);

		EGameDirection direction = FindDirection(this, m_actor_target->GetActorLocation());

		if (direction == EGameDirection::EGD_Front)
		{
			 m_action_state = EActionState_NPC::EASN_Chasing;

			 MoveToTarget(m_actor_target, m_radius_attack * 0.5);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OnChangeVigilanceState : StartTurn"));
			StartTurn(direction);
		}

		//m_action_state = EActionState_NPC::EASN_Chasing;
		//MoveToTarget(m_actor_target);
	}
}

void AC4311::OnMoveCompleted(const FPathFollowingResult& Result)
{
	if (EActionState_NPC::EASN_Chasing == m_action_state)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveComplete"))
	}

	switch (m_action_state)
	{
	case EActionState_NPC::EASN_Patrolling:
		{
			m_action_state = EActionState_NPC::EASN_Unoccupied;

			if (InTargetRange(m_target_patrol, m_radius_patrol))
			{
				StartWatching();
			}
		} break;
	case EActionState_NPC::EASN_Chasing:
		{
			if (InTargetRange(m_actor_target, m_radius_attack)) {
				StartAttack();
			}
		} break;
	}
}

void AC4311::OnNextAttack()
{
	if (m_attack_type_prev > EAttackType::EATKT_Attack ||
		m_attack_strength > EAttackStrength::EATKS_Normal)
		return;

	// 1. 공격 성공인 경우
	// 2. 공격 범위에 대상이 있는 경우
	// 3. 한번 더 공격 or 공격 중지
	if (m_attack_success || InTargetRange(m_actor_target, m_radius_attack) || (1 == FMath::RandRange(0, 1)))
	{
		FName next_section = ChooseNextAttack();
		if (next_section.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("Can not found next attack section."));
			return;
		}

		UAnimInstance* anim_instance = m_mesh->GetAnimInstance();
		if (nullptr != anim_instance)
		{
			anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
			GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

			MoveToTarget(m_actor_target);
		}

		m_equiped_weapon_R->SetAttackWeight(m_equip_state, m_attack_strength, m_attack_type_prev);

		//UE_LOG(LogTemp, Warning, TEXT("Play Montage : Next Attack"));
		PlayMontage(m_montage_attack, next_section);
		return;
	}

	//	2. 경계 자세 - 방어 / 일반
	//		> 플레이어 체력에 따라서 공격형 방어형으로 분리한다.
}

void AC4311::OnEndAttack()
{
	if (m_action_state == EActionState_NPC::EASN_Unoccupied)
	{
		FinishVigilance();
	}
	else if (m_action_state != EActionState_NPC::EASN_Chasing)
	{
		FString state = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));
		//UE_LOG(LogTemp, Warning, TEXT("OnEndAttack StartConfront State : %s"), *state);

		if (InTargetRange(m_actor_target, m_radius_tracking))
		{
			//UE_LOG(LogTemp, Warning, TEXT("OnEndAttack : StartConfront"));
			StartConfront();
		}
	}
}

void AC4311::OnReactEnd()
{
	UGameCharacter_AnimInstance* anim_instance = Cast<UGameCharacter_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID(anim_instance)

	anim_instance->IsNeedBlendPerBone(false);

	CHECK_INVALID(m_actor_target)

	//UE_LOG(LogTemp, Warning, TEXT("Now Unoccupied"))
	m_action_state = EActionState_NPC::EASN_Unoccupied;
	EGameDirection direction = FindDirection(this, m_actor_target->GetActorLocation());

	if (direction == EGameDirection::EGD_Front)
	{
		(EVigilanceState::EVS_Repose == m_vigilance_state) ? StartCombat() : StartConfront();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnReactEnd : StartTurn"));
		StartTurn(direction);
	}
}

void AC4311::OnTurnEnd()
{
	if ((m_death_pose != EDeathPose::EDP_Alive) || IsCantMoveState()) { return; }

	Super::OnTurnEnd();
	UE_LOG(LogTemp, Warning, TEXT("OnTurnEnd "));
	if (m_vigilance_state == EVigilanceState::EVS_Repose)
	{
		StopAllMontage();

		if (nullptr != m_actor_target)
		{
			StartCombat();
		}
		else if (m_is_patrolling)
		{
			m_action_state = EActionState_NPC::EASN_Patrolling;
			
			FVector	target = m_location_assembly_object;
			if (false == m_is_resting)
			{
				CHECK_INVALID(m_target_patrol)
				target = m_target_patrol->GetActorLocation();
			}

			MoveToLocation(target, false == m_is_resting ? -1.f : 0, !(nullptr == m_target_patrol));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("In Else "));
			UCapsuleComponent* capsule = GetCapsuleComponent();
			CHECK_INVALID(capsule)
			capsule->SetCanEverAffectNavigation(true);

			CHECK_INVALID(m_target_assembly_object)
			m_action_state = EActionState_NPC::EASN_Resting;

			FVector assemble_location = m_assembly_point->GetActorLocation();
			FVector assemble_offset_location = FVector(assemble_location.X, assemble_location.Y, GetActorLocation().Z);
			FVector object_location = FVector(m_location_assembly_object.X, m_location_assembly_object.Y, GetActorLocation().Z);
			FVector actor_location = GetActorLocation();

			FVector dir = (assemble_offset_location - actor_location).GetSafeNormal2D();

			//UKismetSystemLibrary::DrawDebugArrow(this, actor_location, actor_location + dir * 40.f, 5.f, FColor::Black, 5.f);

			//SetActorLocation(object_location);

			FString name = m_target_assembly_object->GetInteractionSectionName();
			PlayMontage(m_montage_resting, FName(name), -1.f);
		}
	}
	else
	{
		FString status = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));

		if (InTargetRange(m_actor_target, m_radius_confront))
		{
			StartConfront();
		}
		else if (false == InTargetRange(m_actor_target, m_radius_tracking))
		{
			FinishVigilance();
		}
		else
		{
			OnCheckMissingTarget();
		}
	}
}

void AC4311::OnAttackDefended(const EAttackWeight& attack_weight)
{
	if (m_battle_pose  != EBattlePose::EBP_Defense ||
		m_action_state != EActionState_NPC::EASN_Confronting)
	{ return; }

	FName section_name;
	switch (attack_weight)
	{
	case EAttackWeight::EAW_Small:
	case EAttackWeight::EAW_Medium:		section_name = FName("Guard_Medium");	  break;
	case EAttackWeight::EAW_Heavy:		section_name = FName("Guard_Heavy");	  break;
	case EAttackWeight::EAW_ExtraHeavy:
	case EAttackWeight::EAW_UltraHeavy: section_name = FName("Guard_ExtraHeavy"); break;
	case EAttackWeight::EAW_None: { return; }
	}

	m_action_state = EActionState_NPC::EASN_GuardReact;
	PlayMontage(m_montage_guard, section_name);
}

void AC4311::OnAttackBlocked(const EAttackWeight& attack_weight)
{
	if (m_attack_strength == EAttackStrength::EATKS_Strong) { return; }

	FName section_name = FName("Blocked_Normal");

	PlayMontage(m_montage_blocked, section_name);
}

void AC4311::OnTakeStunn()
{
	Super::OnTakeStunn();

	m_action_state = EActionState_NPC::EASN_Stunning;

	PlayMontage(m_montage_stunn, FName("Stunned"));
}

void AC4311::OnTakeExecutionEnd()
{
	Super::OnTakeExecutionEnd();

	m_action_state = EActionState_NPC::EASN_Unoccupied;
	if (nullptr != Cast<AShield_Actor>(m_equiped_weapon_L)) { m_battle_pose = EBattlePose::EBP_Defense; }
}
