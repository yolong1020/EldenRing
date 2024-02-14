// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C4311/C4311.h"
#include "Characters/NPC/GameCharacter_AnimInstance.h"
#include "Characters/Component/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameBase/ERGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "HUD/Status/HealthBarComponent.h"
#include "Items/Weapons/MeleeAttack_Actor.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Items/Weapons/Shield_Actor.h"
#include "FieldObject/PatrolPoint/PatrolPoint.h"
#include "FieldObject/AssemblyPoint/AssemblyPoint.h"
#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "System/ObserverManager.h"
#include "System/GameObjectManager.h"
#include "Animation/AnimLinkableElement.h"
#include "TimerManager.h"
#include "Common/CommonFunctions.h"
#include "Macro/DebugMacros.h"
#include "Macro/BasicMacro.h"

AC4311::AC4311()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AC4311::Destroyed()
{
	Super::Destroyed();

	if (m_target_assembly) { m_target_assembly->ResetPointObject(); }
	if (m_assembly_point)  { m_assembly_point->UnregistAssemblyPointMember(this); }
}

void AC4311::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Monster"));

	EquipWeaponToStart();

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)
	m_observer_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_SMART_PTR(m_observer_mgr)
	m_observer_mgr->RegistObserver<AC4311>(EObserverEventType::EOET_SaveData, this, &AC4311::SaveCharacterData);
}

void AC4311::OnDeath(TWeakObjectPtr<UAnimMontage> Montage, const float& Duration)
{
	UGameCharacter_AnimInstance* anim_instance = Cast<UGameCharacter_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID_PTR(anim_instance)
	anim_instance->SetHitReactProperties(false, 0, EAttackWeight::EAW_None, EHitCycleType::EHCT_None);

	Super::OnDeath(Montage, Duration);
}

void AC4311::GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight)
{
	CHECK_INVALID_PTR(m_attribute)
	CHECK_INVALID_PTR(m_widget_healthbar)

	bool is_alive = m_attribute->IsAlive();
	m_widget_healthbar->SetVisibility(is_alive ? true : false);

	if (is_alive)
	{
		UCharacterMovementComponent* movement_component = GetCharacterMovement();
		CHECK_INVALID_PTR(movement_component)
		movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;

		StopAllMontage();
		HitReact(Dir, AttackWeight);

		if (m_equiped_weapon_R && m_equiped_weapon_R->GetAttachSocketName() == FName("R_HandSocket"))
		{
			m_vigilance_state = EVigilanceState::EVS_Vigilance; 
		}
	}
	else
	{
		if (EDeathPose::EDP_Alive != m_death_pose) return;

		FName section_name;
		switch (FMath::RandRange(0, m_montage_death->GetNumSections() - 1))
		{
			case 0: section_name = FName("Death_Front"); m_death_pose = EDeathPose::EDP_Death_Front; break;
			case 1: section_name = FName("Death_Back");  m_death_pose = EDeathPose::EDP_Death_Back; break;
		}

		StopAllMontage();
		PlayMontageSection(m_montage_death, section_name);
		OnDeath(m_montage_death);
	}

	if (m_sound_hit)	{ UGameplayStatics::PlaySoundAtLocation(this, m_sound_hit, ImpactPoint); }
	if (m_particle_hit) { UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_particle_hit, ImpactPoint); }
}

void AC4311::SetWeaponCollision(ECollisionEnabled::Type type)
{
	AMeleeAttack_Actor* weapon = Cast<AMeleeAttack_Actor>(m_equiped_weapon_R);
	if (!weapon) return;
	weapon->SetWeaponCollision(type);
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
		m_observer_mgr->RegistDeathObserver<AC4311>(m_actor_target, this, &AC4311::AfterTargetDeath);
	}

	return DamageAmount;
}

void AC4311::EquipWeaponToStart()
{
	if (m_class_weapon_R) { EquipWeapon("Weapon_SteathSocket", EWeaponEquipHand::EWEH_Right); }
	if (m_class_weapon_L) { EquipWeapon("L_Hand_Shield", EWeaponEquipHand::EWEH_Left); }
}

void AC4311::HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight)
{
	CHECK_INVALID_PTR(m_anim_instance)

	bool		is_need_blend		= true;
	FString		hit_section_postfix	= "";
	int32		selected_index		= 0;

	UAnimMontage*	montage_with_direction	= nullptr;
	UAnimMontage*	montage_no_direction	= nullptr;

	switch (attack_weight)
	{
		case EAttackWeight::EAW_Small:
		{
			hit_section_postfix	= FString("Small");
		}break;
		case EAttackWeight::EAW_Medium:
		{
			hit_section_postfix	= FString("Medium");
			montage_no_direction	= m_montage_hit_medium;
			montage_with_direction	= m_montage_hit_medium_direction;
		} break;
		case EAttackWeight::EAW_Heavy:
		case EAttackWeight::EAW_ExtraHeavy:
		case EAttackWeight::EAW_UltraHeavy: 
		{
			hit_section_postfix	= FString("UltraHeavy");
			montage_with_direction	= nullptr;

			PlayMontageSection(m_montage_hit_ultra_heavy_directiony, FName(hit_section_postfix));

			UGameCharacter_AnimInstance* anim_instance = Cast<UGameCharacter_AnimInstance>(GetMesh()->GetAnimInstance());
			CHECK_INVALID_PTR(anim_instance)
			anim_instance->SetHitReactProperties(is_need_blend, selected_index, attack_weight, EHitCycleType::EHCT_Even);
			anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
		}
	}

	if (hit_section_postfix != FString("UltraHeavy"))
	{
		CHECK_INVALID_PTR(montage_no_direction)
		CHECK_INVALID_PTR(montage_with_direction)

		selected_index = FMath::RandRange(0, montage_no_direction->GetNumSections() - 1);
		switch (hit_direction)
		{
			case EGameDirection::EGD_Back:	hit_section_postfix += FString("_Back");  break;
			case EGameDirection::EGD_Left:	hit_section_postfix += FString("_Left");  break;
			case EGameDirection::EGD_Right:	hit_section_postfix += FString("_Right"); break;
			case EGameDirection::EGD_Front: 
			{
				is_need_blend	    = false;
				selected_index	    = FMath::RandRange(1, montage_no_direction->GetNumSections());
				hit_section_postfix += (FString("_0") + FString::FromInt(selected_index));
			} break;
			default: return;
		}

		UGameCharacter_AnimInstance* anim_instance = Cast<UGameCharacter_AnimInstance>(GetMesh()->GetAnimInstance());
		CHECK_INVALID_PTR(anim_instance)

		EHitCycleType cycle_type = anim_instance->GetHitCycle();
		anim_instance->SetHitReactProperties(is_need_blend, selected_index, attack_weight, cycle_type == EHitCycleType::EHCT_Even ? EHitCycleType::EHCT_Odd : EHitCycleType::EHCT_Even);
		anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);

		if (hit_direction != EGameDirection::EGD_Front) { PlayMontageSection(montage_with_direction, FName(hit_section_postfix)); }
		else											{ PlayMontageSection(montage_no_direction, FName(hit_section_postfix)); }
		
		bool is_already_clear = false;
		for (FAnimMontageInstance* instance : m_anim_instance->MontageInstances)
		{
			if (false   == instance->bPlaying  ||
			    nullptr == instance->Montage   ||
			    false   == instance->Montage->GetFName().ToString().Contains("AM_HitReact")) continue;

			int32 index = instance->Montage->GetSectionIndex(FName(hit_section_postfix));
			if (0 > index) continue;

			FCompositeSection section	= instance->Montage->GetAnimCompositeSection(index);
			int32		  segment_index = section.GetSegmentIndex();

			if (false == instance->Montage->SlotAnimTracks.IsValidIndex(0)) continue;

			FSlotAnimationTrack& default_slot = instance->Montage->SlotAnimTracks[0];
			if (false == default_slot.AnimTrack.AnimSegments.IsValidIndex(segment_index)) continue;

			FAnimSegment segment = default_slot.AnimTrack.AnimSegments[segment_index];
			float	     length  = segment.GetLength();

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
	AGameCharacter* target = Cast<AGameCharacter>(seen_pawn);
	if (!target || IsCantMoveState()) return;

	if (!m_actor_target && target->IsAlive())
	{
		if (seen_pawn->ActorHasTag(FName("Player")))
		{
			StartCombat();
			m_actor_target = target;
			m_combat_list.Add(target);

			m_observer_mgr->RegistDeathObserver<AC4311>(m_actor_target, this, &AC4311::AfterTargetDeath);
		}
	}
	else if (target == m_actor_target && EVigilanceState::EVS_Vigilance == m_vigilance_state)
	{
		OnCheckMissingTarget();

		if (IsInTargetRange(m_actor_target, m_radius_tracking))
		{
			if (IsInTargetRange(m_actor_target, m_radius_confront) == false)
			{
				if (m_action_state != EActionState_NPC::EASN_Attacking &&
				    m_action_state != EActionState_NPC::EASN_TakeExecution)
				{
					ChangeRootMotionMode(ERootMotionMode::RootMotionFromEverything);
					MoveToTarget(m_actor_target, m_radius_attack * 0.5);
				}

				m_action_state = EActionState_NPC::EASN_Chasing;
				if (nullptr != Cast<AShield_Actor>(m_equiped_weapon_L)) { m_battle_pose = EBattlePose::EBP_Defense; }

				GetWorldTimerManager().ClearTimer(m_timer_reserve_action);
			}
		}
		else
		{
			if (m_action_state != EActionState_NPC::EASN_Attacking &&
			    m_action_state != EActionState_NPC::EASN_TakeExecution)
			{
				FinishVigilance();
			}
		}
	}
}

void AC4311::FinishFadeOut(UNiagaraComponent* PSystem)
{
	Super::FinishFadeOut(PSystem);
	
	UninitAndDestroy();
}

void AC4311::StartWatching()
{
	Super::StartWatching();

	if (40.f >= FMath::Rand() % 100)
	{
		PlayMontageSection(m_montage_watching, FName("Watching_Switch"), -1);
		m_is_idle_to_watching = true;
	}
	else
	{
		const float wait_sec = FMath::RandRange(m_time_wait_min, m_time_wait_max);
		GetWorldTimerManager().SetTimer(m_timer_watching, this, &AC4311::MoveToPatrolPoint, wait_sec);
	}
}

void AC4311::StartCombat()
{
	if (m_vigilance_state == EVigilanceState::EVS_Vigilance	|| IsCantMoveState()) return;

	GetWorldTimerManager().ClearTimer(m_timer_watching);
	GetWorldTimerManager().ClearTimer(m_timer_patrol);

	PlayMontageSection(m_montage_swap, FName("Swap_Battle"));

	m_is_resting    = false;
	m_is_patrolling = false;
	m_action_state  = EActionState_NPC::EASN_Swap;
}

void AC4311::StartAttack()
{
	if (m_equiped_weapon_L && Cast<AShield_Actor>(m_equiped_weapon_L))
	{
		IWeapon_Interface* shield = Cast<IWeapon_Interface>(m_equiped_weapon_L);
		shield->SetShieldMod(false);
		shield->SetWeaponCollision(ECollisionEnabled::NoCollision);
	}

	FString status = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));

	m_action_state		= EActionState_NPC::EASN_Attacking;
	m_battle_pose		= EBattlePose::EBP_Attack;
	m_attack_strength	= ((FMath::Rand() % 100) > 50) ? EAttackStrength::EATKS_Normal : EAttackStrength::EATKS_Strong;
	m_attack_type_prev	= EAttackType::EATKT_Attack;

	FString section_name = "";
	if (m_attack_strength != EAttackStrength::EATKS_Normal) 
	{ 
		if ((FMath::Rand() % 100) > 50) { section_name = FString("Strong_Attack_"); }
		else				{ section_name = FString("Dash_Attack"); }
	}
	else { section_name = FString("Attack_"); }

	if (section_name != FString("Dash_Attack")) 
	{
		if ((FMath::Rand() % 100) > 50) { section_name += FString("01"); }
		else				{ section_name += FString("02"); }
	}

	CHECK_INVALID_PTR(m_equiped_weapon_R)
	m_equiped_weapon_R->SetAttackWeight(m_equip_state, m_attack_strength, m_attack_type_prev);

	PlayMontageSection(m_montage_attack, FName(section_name));

	m_attack_name_prev = FName(section_name);
}

void AC4311::StartTurn(const EGameDirection& direction)
{
	if (nullptr != Cast<AShield_Actor>(m_equiped_weapon_L)) m_battle_pose = EBattlePose::EBP_Defense;

	FString pre_fix = FString();
	switch (m_battle_pose)
	{
		case EBattlePose::EBP_Attack:	pre_fix = FString("Battle_"); break;
		case EBattlePose::EBP_Defense:	pre_fix = FString("Shield_"); break;
	}

	FString post_fix = FString();
	switch (direction)
	{
		case EGameDirection::EGD_Back:	post_fix = FString("Back");  break;
		case EGameDirection::EGD_Left:	post_fix = FString("Left");  break;
		case EGameDirection::EGD_Right: post_fix = FString("Right"); break;
	}

	PlayMontageSection(m_montage_turn, FName(pre_fix + post_fix));
}

void AC4311::StartConfront()
{
	if (!m_actor_target || m_action_state == EActionState_NPC::EASN_HitReact) return;

	m_action_state	= EActionState_NPC::EASN_Confronting;
	if (Cast<AShield_Actor>(m_equiped_weapon_L)) 
	{ 
		m_battle_pose = EBattlePose::EBP_Defense;
		m_equiped_weapon_L->SetWeaponCollision(ECollisionEnabled::QueryOnly);
	}

	EGameDirection direction = (EGameDirection)FMath::RandRange((int32)EGameDirection::EGD_Back, (int32)EGameDirection::EGD_Right);
	
	bool is_too_close = IsInTargetRange(m_actor_target, m_radius_attack);
	if (is_too_close) { direction = EGameDirection::EGD_Back; }

	FString section_name;
	switch (direction)
	{
		case EGameDirection::EGD_Back:	section_name = FString("ToBack");  break;
		case EGameDirection::EGD_Front:	section_name = FString("ToFront"); break;
		case EGameDirection::EGD_Left:	section_name = FString("ToLeft");  break;
		case EGameDirection::EGD_Right:	section_name = FString("ToRight"); break;
	}

	if (Cast<AShield_Actor>(m_equiped_weapon_L) != nullptr) { section_name += FString("_Shield"); }

	PlayMontageSection(m_montage_confront, FName(section_name));

	CHECK_INVALID_PTR(m_anim_instance)
	m_anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

	MoveToTarget(m_actor_target);

	float wait_sec = FMath::RandRange(1.f, 2.f);
	GetWorldTimerManager().SetTimer(m_timer_reserve_action, this, &AC4311::ChooseNextAction, wait_sec);
}

void AC4311::OnCheckMissingTarget()
{
	if (IsInSight(m_actor_target))
	{
		GetWorldTimerManager().SetTimer(m_timer_sight, this, &AC4311::OnCheckMissingTarget, m_sec_kwon_missing_target);
	}
	else if (IsInTargetRange(m_actor_target, m_radius_tracking))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCheckMissingTarget : StartTurn"));
		EGameDirection direction = FCommonFunctions::FindDirection(this, m_actor_target->GetActorLocation());

		if (direction != EGameDirection::EGD_Front && false == IsCantMoveState())
		{
			StartTurn(direction);
		}
	}
	else if (false == IsInTargetRange(m_actor_target, m_radius_tracking))
	{
		GetWorldTimerManager().ClearTimer(m_timer_sight);
		FinishVigilance();
	}
}

void AC4311::OnWatchingFinished()
{
	PlayMontageSection(m_montage_watching, FName("Watching_Switch"));
	m_is_idle_to_watching = false;
}

void AC4311::OnWatchingSwitch()
{
	if (m_is_idle_to_watching)
	{
		m_action_state = EActionState_NPC::EASN_Watching;

		const float wait_sec = FMath::RandRange(m_time_wait_min, m_time_wait_max);
		GetWorldTimerManager().SetTimer(m_timer_watching, this, &AC4311::OnWatchingFinished, wait_sec);
	}
	else { MoveToPatrolPoint(); }
}

void AC4311::OnTurnEnd()
{
	if (IsCantMoveState()) return;

	Super::OnTurnEnd();

	if (m_vigilance_state == EVigilanceState::EVS_Repose)
	{
		StopAllMontage();

		if (m_actor_target)
		{
			StartCombat();
		}
		else if (m_is_patrolling)
		{
			m_action_state = EActionState_NPC::EASN_Patrolling;
			
			UE_LOG(LogTemp, Warning, TEXT("%s OnTurnEnd -> MoveToLocation"), *m_unique_name)
			FVector	target = m_target_patrol->GetActorLocation();
			MoveToLocation(target, -1.f, true);
		}
		else if (m_target_assembly)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s OnTurnEnd -> MoveToAssemblePoint"), *m_unique_name)
			MoveToAssemblePoint(m_target_assembly);
		}
	}
	else
	{
		if (IsInTargetRange(m_actor_target, m_radius_confront))
		{
			StartConfront();
		}
		else if (false == IsInTargetRange(m_actor_target, m_radius_tracking))
		{
			FinishVigilance();
		}
		else
		{
			OnCheckMissingTarget();
		}
	}
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
			if (IsInTargetRange(m_actor_target, m_radius_tracking))
			{
				if (IsInTargetRange(m_actor_target, m_radius_confront) == false)
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

	PlayMontageSection(m_montage_swap, FName("Swap_Repose"));
	StopAllMovement();

	if (nullptr == m_widget_healthbar) return;
	m_widget_healthbar->SetVisibility(false);

	if (m_equiped_weapon_L && Cast<AShield_Actor>(m_equiped_weapon_L))
	{
		IWeapon_Interface* shield = Cast<IWeapon_Interface>(m_equiped_weapon_L);
		shield->SetShieldMod(false);
		shield->SetWeaponCollision(ECollisionEnabled::NoCollision);
	}
}

const bool AC4311::IsCantMoveState()
{
	return (m_action_state == EActionState_NPC::EASN_HitReact 	||
		m_action_state == EActionState_NPC::EASN_GuardReact 	||
		m_action_state == EActionState_NPC::EASN_Swap 		||
		m_action_state == EActionState_NPC::EASN_TakeExecution 	||
		m_action_state == EActionState_NPC::EASN_Stunning 	||
		m_death_pose   != EDeathPose::EDP_Alive);
}

void AC4311::AfterTargetDeath(TObjectPtr<AGameCharacter> Target)
{
	Super::AfterTargetDeath(Target);

	if (!m_combat_list.IsEmpty())
	{
		AGameCharacter* character = *m_combat_list.FindByPredicate([&](AGameCharacter* const Object)->bool {return Object == Target; });
		m_combat_list.Remove(character);
	}
	
	if (m_combat_list.IsEmpty())
	{
		FLatentActionInfo callback;
		callback.UUID			= FGuid::NewGuid().A;
		callback.CallbackTarget 	= this;
		callback.Linkage		= 0;
		callback.ExecutionFunction 	= FName("FinishVigilance");

		float duration = WaitAfterPlayerDeath;
		duration = (float)FMath::RandRange(1.f, duration);
		UKismetSystemLibrary::Delay(this, duration, callback);
	}
}

void AC4311::GameStartAction()
{
	Super::GameStartAction();
	
	m_action_state = EActionState_NPC::EASN_Patrolling;
	StartPatrol();
}

void AC4311::OnSwapWeaponR()
{
	CHECK_INVALID_PTR(m_equiped_weapon_R);

	FName socket_name = (m_vigilance_state == EVigilanceState::EVS_Repose) ? FName("R_HandSocket") : FName("Weapon_SteathSocket");
	m_equiped_weapon_R->Equip(GetMesh(), socket_name, this, this);
}

void AC4311::OnChangeVigilanceState(const EVigilanceState& state)
{
	m_action_state = (m_action_state == EActionState_NPC::EASN_Swap) ? EActionState_NPC::EASN_Unoccupied : m_action_state;
	if (IsCantMoveState()) return;

	Super::OnChangeVigilanceState(state);

	if (EVigilanceState::EVS_Repose == state)
	{
		m_action_state = EActionState_NPC::EASN_Unoccupied;
		m_battle_pose  = EBattlePose::EBP_Unoccupied;

		if (m_assembly_point && m_target_assembly && m_target_assembly->UseAgainPointObject())
		{
			MoveToAssemblePoint(m_target_assembly);
		}
		else
		{
			StartPatrol();
		}
	}
	else if (EVigilanceState::EVS_Vigilance == state)
	{
		CHECK_INVALID_PTR(m_widget_healthbar)
		m_widget_healthbar->SetVisibility(true);

		EGameDirection direction = FCommonFunctions::FindDirection(this, m_actor_target->GetActorLocation());

		if (direction == EGameDirection::EGD_Front)
		{
			 m_action_state = EActionState_NPC::EASN_Chasing;
			 MoveToTarget(m_actor_target, m_radius_attack * 0.5);
		}
		else
		{
			StartTurn(direction);
		}
	}
}

void AC4311::OnMoveCompleted(const FPathFollowingResult& Result)
{
	switch (m_action_state)
	{
		case EActionState_NPC::EASN_Patrolling:
			{
				m_action_state = EActionState_NPC::EASN_Unoccupied;
				if (IsInTargetRange(m_target_patrol, m_radius_patrol)) { StartWatching(); }
			} break;
		case EActionState_NPC::EASN_Chasing:
			{
				if (IsInTargetRange(m_actor_target, m_radius_attack)) { StartAttack(); }
			} break;
	}
}

void AC4311::OnNextAttack()
{
	if (m_attack_type_prev > EAttackType::EATKT_Attack ||
	    m_attack_strength > EAttackStrength::EATKS_Normal) return;

	if (m_attack_success || IsInTargetRange(m_actor_target, m_radius_attack) || (1 == FMath::RandRange(0, 1)))
	{
		FName next_section = ChooseNextAttack();
		if (next_section.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("Can not found next attack section."));
			return;
		}

		CHECK_INVALID_PTR(m_anim_instance)
		m_anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
		MoveToTarget(m_actor_target);

		m_equiped_weapon_R->SetAttackWeight(m_equip_state, m_attack_strength, m_attack_type_prev);

		PlayMontageSection(m_montage_attack, next_section);
	}
}

void AC4311::OnEndAttack()
{
	if (m_action_state == EActionState_NPC::EASN_Unoccupied)
	{
		FinishVigilance();
	}
	else if (m_action_state != EActionState_NPC::EASN_Chasing && IsInTargetRange(m_actor_target, m_radius_tracking))
	{
		StartConfront();
	}
}

void AC4311::OnReactEnd()
{
	UGameCharacter_AnimInstance* anim_instance = Cast<UGameCharacter_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID_PTR(anim_instance)
	anim_instance->IsNeedBlendPerBone(false);

	CHECK_INVALID_PTR(m_actor_target)
	EGameDirection direction = FCommonFunctions::FindDirection(this, m_actor_target->GetActorLocation());

	m_action_state = EActionState_NPC::EASN_Unoccupied;

	if (direction == EGameDirection::EGD_Front)
	{
		(EVigilanceState::EVS_Repose == m_vigilance_state) ? StartCombat() : StartConfront();
	}
	else
	{
		StartTurn(direction);
	}
}

void AC4311::OnAttackDefended(const EAttackWeight& attack_weight)
{
	if (m_battle_pose  != EBattlePose::EBP_Defense || m_action_state != EActionState_NPC::EASN_Confronting) return;

	FName section_name;
	switch (attack_weight)
	{
		case EAttackWeight::EAW_Small:
		case EAttackWeight::EAW_Medium:	    section_name = FName("Guard_Medium");	break;
		case EAttackWeight::EAW_Heavy:	    section_name = FName("Guard_Heavy");	break;
		case EAttackWeight::EAW_ExtraHeavy:
		case EAttackWeight::EAW_UltraHeavy: section_name = FName("Guard_ExtraHeavy");	break;
		default: return;
	}

	IWeapon_Interface* shield = Cast<IWeapon_Interface>(m_equiped_weapon_L);
	if (shield)
	{
		shield->SetShieldMod(true);
	}

	m_action_state = EActionState_NPC::EASN_GuardReact;
	PlayMontageSection(m_montage_guard, section_name);
}

void AC4311::OnAttackBlocked(const EAttackWeight& attack_weight)
{
	if (m_attack_strength == EAttackStrength::EATKS_Strong) { return; }

	FName section_name = FName("Blocked_Normal");

	PlayMontageSection(m_montage_blocked, section_name);
}

void AC4311::OnTakeStunn()
{
	Super::OnTakeStunn();

	m_action_state = EActionState_NPC::EASN_Stunning;
	PlayMontageSection(m_montage_stunn, FName("Stunned"));
}

void AC4311::OnTakeExecutionEnd()
{
	Super::OnTakeExecutionEnd();

	m_action_state = EActionState_NPC::EASN_Unoccupied;
	if (nullptr != Cast<AShield_Actor>(m_equiped_weapon_L)) { m_battle_pose = EBattlePose::EBP_Defense; }
}
