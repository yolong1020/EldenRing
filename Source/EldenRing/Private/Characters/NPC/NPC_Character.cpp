// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/NPC_Character.h"
#include "Characters/NPC/GameCharacter_AnimInstance.h"
#include "Characters/NPC/GameCharacter_AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HUD/Status/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "FieldObject/AssemblyPoint/AssemblyPoint.h"
#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "Basic/DebugMacros.h"

ANPC_Character::ANPC_Character()
{
	m_widget_healthbar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	m_widget_healthbar->SetupAttachment(GetRootComponent());

	m_pawn_sensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	m_pawn_sensing->SightRadius = 2000.0f;
	m_pawn_sensing->SetPeripheralVisionAngle(45.0f);
}

void ANPC_Character::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("NPC"));

	CHECK_INVALID(m_widget_healthbar)
	m_widget_healthbar->SetVisibility(false);
	RefreshHealthbar();

	m_controller_ai = Cast<AGameCharacter_AIController>(GetController());
	CHECK_INVALID(m_controller_ai)

	CHECK_INVALID(m_pawn_sensing)
	m_pawn_sensing->OnSeePawn.AddDynamic(this, &ANPC_Character::PawnSeen);
}

void ANPC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPC_Character::Destroyed()
{
	Super::Destroyed();

	if (nullptr != m_equiped_weapon_R) { m_equiped_weapon_R->Destroy(); }
	if (nullptr != m_equiped_weapon_L) { m_equiped_weapon_L->Destroy(); }
}

float ANPC_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	RefreshHealthbar(DamageAmount);

	return DamageAmount;
}

void ANPC_Character::TakeExecution(AActor* target, const EGameDirection& direction, const int32& damage)
{
	AGameCharacter* target_character = Cast<AGameCharacter>(target);
	if (nullptr == m_controller_ai || nullptr == target_character) return;

	m_actor_target = target_character;

	UGameCharacter_AnimInstance* anim_instance = Cast<UGameCharacter_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID(anim_instance)
	anim_instance->SetHitReactProperties(false, 0, EAttackWeight::EAW_None, EHitCycleType::EHCT_None);

	FVector target_location = target_character->GetActorLocation();
	FVector new_location	= target_location + target_character->GetActorForwardVector() * 10.f;
	float	location_offset = 0;

	FRotator new_rotation;
	if (direction == EGameDirection::EGD_Front)
	{
		new_rotation	 = target_character->GetActorRotation();
		new_rotation.Yaw += 180.f;

		location_offset	 = 1.f;
	}
	else
	{
		new_rotation	= target_character->GetActorRotation();
		location_offset = -3.f;
	}

	new_location = new_location + target_character->GetActorRightVector() * location_offset;
	SetActorLocationAndRotation(new_location, new_rotation);

	m_action_state = EActionState_NPC::EASN_TakeExecution;

	CHECK_INVALID(m_widget_healthbar)
	m_widget_healthbar->SetVisibility(true);
	m_attribute->ReceiveDamage(damage);
	m_widget_healthbar->SetHealthPercent(m_attribute->GetHealthPercent());

	FName section_name = direction == EGameDirection::EGD_Front ? FName("TakeExecution_Front") : FName("TakeExecution_Back");
	PlayMontage(m_montage_take_execution, section_name);

	if (m_battle_pose == EBattlePose::EBP_Unoccupied) { m_actor_target = target_character; }
	ClearAllTimer();

	SetWeaponCollision(ECollisionEnabled::NoCollision);
}

bool ANPC_Character::IsCurrentActionState(const FString& action)
{
	if (action == "Attacking") { return m_action_state == EActionState_NPC::EASN_Attacking; }
	return false;
}

bool ANPC_Character::IsGuardState()
{
	FString battle_pose  = StaticEnum<EBattlePose>()->GetNameStringByValue(int64(m_battle_pose));
	FString action_state = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));

	return m_action_state == EActionState_NPC::EASN_Confronting && m_battle_pose == EBattlePose::EBP_Defense;
}

void ANPC_Character::SetBlindPawn(const bool& is_blind)
{
	CHECK_INVALID(m_pawn_sensing)

	if (is_blind)	{ m_pawn_sensing->OnSeePawn.RemoveDynamic(this, &ANPC_Character::PawnSeen); }
	else		{ m_pawn_sensing->OnSeePawn.AddDynamic(this, &ANPC_Character::PawnSeen); }
}

bool ANPC_Character::EquipWeapon(const FName& socket_name, const EWeaponEquipHand& hand)
{
	if (hand == EWeaponEquipHand::EWEH_Right)
	{
		if (nullptr == m_equiped_weapon_R)
		{
			UWorld* world = GetWorld();
			if (nullptr == world) return false;

			AWeapon_Actor* weapon = world->SpawnActor<AWeapon_Actor>(m_class_weapon_R);
			if (nullptr == weapon) return false;

			m_equiped_weapon_R = weapon;
		}

		m_equiped_weapon_R->Equip(GetMesh(), socket_name, this, this);
	}
	else
	{
		if (nullptr == m_equiped_weapon_L)
		{
			UWorld* world = GetWorld();
			if (nullptr == world) return false;

			AWeapon_Actor* weapon = world->SpawnActor<AWeapon_Actor>(m_class_weapon_L);
			if (nullptr == weapon) return false;

			m_equiped_weapon_L = weapon;
		}

		m_equiped_weapon_L->Equip(GetMesh(), socket_name, this, this);
	}

	return true;
}

void ANPC_Character::InitAssemblyPointObject(AAssemblePointObject* const point_object)
{
	m_target_assembly_object = point_object;
	m_action_state		 = EActionState_NPC::EASN_Resting;
}

void ANPC_Character::OnChangeVigilanceState(const EVigilanceState& state)
{
	m_vigilance_state = state;
}

void ANPC_Character::OnTurnEnd()
{
	if (nullptr == m_controller_ai) return;

	FVector position = GetActorLocation();
	FVector direction = GetActorForwardVector();
	if (!direction.IsNearlyZero()) { direction.Normalize(); }
	direction *= GetMovementComponent()->GetMaxSpeed();

	m_controller_ai->SetControlRotation(direction.Rotation());
	m_controller_ai->StopMovement();
}

void ANPC_Character::OnEndStunn()
{
	Super::OnEndStunn();

	m_action_state = EActionState_NPC::EASN_Unoccupied;
	SetWeaponCollision(ECollisionEnabled::QueryOnly);
}

void ANPC_Character::OnDeathCompleted()
{
	SetLifeSpan(3.f);

	m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANPC_Character::OnSwapWeaponR()
{
	CHECK_INVALID(m_equiped_weapon_R);

	FName socket_name = (m_vigilance_state == EVigilanceState::EVS_Repose) ? FName("R_HandSocket") : FName("Weapon_SteathSocket");
	m_equiped_weapon_R->Equip(GetMesh(), socket_name, this, this);
}

bool ANPC_Character::IsNeedChangeDeath(const EGameDirection& direction)
{
	if (m_attribute->IsAlive() || nullptr == m_montage_take_execution) return false;

	FName section_name;
	if (direction == EGameDirection::EGD_Front) { section_name = FName("TakeExecution_Death_Front"); m_death_pose = EDeathPose::EDP_Death_Front; }
	else					    { section_name = FName("TakeExecution_Death_Back");  m_death_pose = EDeathPose::EDP_Death_Back;}
	
	SetBlindPawn(true);
	ClearAllTimer();

	PlayMontage(m_montage_take_execution, section_name);
	m_action_state = EActionState_NPC::EASN_Unoccupied;

	if (m_widget_healthbar) { m_widget_healthbar->SetVisibility(false); }

	return true;
}

void ANPC_Character::StartWatching()
{
	if (false == m_is_resting) { m_target_patrol = SelectPatrolTarget(); }
}

void ANPC_Character::ClearAllTimer()
{
	GetWorldTimerManager().ClearTimer(m_timer_sight);
	GetWorldTimerManager().ClearTimer(m_timer_watching);
	GetWorldTimerManager().ClearTimer(m_timer_patrol);
	GetWorldTimerManager().ClearTimer(m_timer_reserve_action);
	GetWorldTimerManager().ClearTimer(m_timer_hit);
}

void ANPC_Character::StopAllMovement()
{
	if (nullptr != m_controller_ai) { m_controller_ai->StopMovement(); }
}

void ANPC_Character::OnMoveOtherPatrolPoint()
{
	FVector	target;
	if (m_is_resting)
	{
		AAssemblePointObject* object 	= m_assembly_point->TryUsePointObject(this);
		CHECK_INVALID(object)

		m_target_patrol			= nullptr;
		m_target_assembly_object	= object;
		m_location_assembly_object	= object->GetActionLocation();
		target 				= m_location_assembly_object;
	}

	if (nullptr != m_target_patrol) { target = m_target_patrol->GetActorLocation(); }

	EGameDirection	direction = FindDirection(this, target);
	if (direction == EGameDirection::EGD_Front)
	{
		m_action_state = EActionState_NPC::EASN_Patrolling;
		MoveToLocation(target, false == m_is_resting ? -1.f : 0, !(nullptr == m_target_patrol));
	}
	else if (direction != EGameDirection::EGD_None)
	{
		StartTurn(direction);
	}
}

void ANPC_Character::RefreshHealthbar(float DamageAmount)
{
	if (nullptr == m_attribute || nullptr == m_widget_healthbar) return;

	m_attribute->ReceiveDamage(DamageAmount);
	m_widget_healthbar->SetHealthPercent(m_attribute->GetHealthPercent());
}

bool ANPC_Character::IsInSight(AActor* target)
{
	return m_pawn_sensing->CouldSeePawn(Cast<APawn>(target));
}

bool ANPC_Character::InTargetRange(AActor* target, double radius)
{
	if (nullptr == target) return false;

	const double distance = (target->GetActorLocation() - GetActorLocation()).Size();

	return distance <= radius;
}

void ANPC_Character::MoveToTarget(AActor* target, const float& accept_radius)
{
	if (nullptr == m_controller_ai || nullptr == target) { return; }

	FVector direction = target->GetActorLocation() - GetActorLocation();
	if (!direction.IsNearlyZero()) { direction.Normalize(); }
	direction *= GetMovementComponent()->GetMaxSpeed();

	m_controller_ai->SetControlRotation(direction.Rotation());
	m_controller_ai->MoveToActor(target, accept_radius);
}

void ANPC_Character::MoveToLocation(const FVector& dest, const float& acceptance_radius, const bool& stop_overlap)
{
	if (nullptr == m_controller_ai) { return; }

	FVector direction = dest - GetActorLocation();
	if (!direction.IsNearlyZero()) { direction.Normalize(); }
	direction *= GetMovementComponent()->GetMaxSpeed();

	m_controller_ai->SetControlRotation(direction.Rotation());
	m_controller_ai->MoveToLocation(dest, acceptance_radius, stop_overlap);
}

AActor* ANPC_Character::SelectPatrolTarget()
{
	if (0 >= m_targets_patrol.Num()) return nullptr;

	TArray<AActor*> valid_target;
	for (const auto& point : m_targets_patrol)
	{
		if (point != m_target_patrol) { valid_target.AddUnique(point); }
	}

	if (0 >= valid_target.Num()) return nullptr;

	int selected = FMath::RandRange(0, valid_target.Num() - 1);
	return valid_target[selected];
}

