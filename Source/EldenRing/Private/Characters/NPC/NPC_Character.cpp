// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/NPC_Character.h"
#include "Characters/NPC/GameCharacter_AnimInstance.h"
#include "Characters/NPC/GameCharacter_AIController.h"
#include "Characters/Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HUD/Status/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "FieldObject/PatrolPoint/PatrolPoint.h"
#include "FieldObject/AssemblyPoint/AssemblyPoint.h"
#include "FieldObject/AssemblyPoint/AssemblePointObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Common/CommonFunctions.h"
#include "GameBase/ERGameInstance.h"
#include "Macro/DebugMacros.h"

ANPC_Character::ANPC_Character()
{
	m_widget_healthbar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	m_widget_healthbar->SetupAttachment(GetRootComponent());

	m_pawn_sensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	m_pawn_sensing->SetPeripheralVisionAngle(45.0f);
}

void ANPC_Character::SetWeaponClassTypes(const TArray<TSubclassOf<class AWeapon_Actor>>& WeaponTypes)
{
	if (WeaponTypes.IsEmpty()) return;

	if (2 < WeaponTypes.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Saved Weapon Types."))
		return;
	}

	m_class_weapon_R = WeaponTypes[0].Get();
	if (WeaponTypes.IsValidIndex(1)) { m_class_weapon_L = WeaponTypes[1].Get(); }

	EquipWeaponToStart();
}

void ANPC_Character::SetPatrolPoints(const TSet<FString>& Points)
{
	TArray<AActor*> actor_points;
	UGameplayStatics::GetAllActorsWithTag(this, FName("PatrolPoint"), actor_points);

	for (AActor* const point : actor_points)
	{
		APatrolPoint* patrol = Cast<APatrolPoint>(point);
		if (!patrol) continue;

		if (Points.Contains(patrol->GetUniqueName()))
		{
			m_targets_patrol.Add(patrol);
		}
	}

	if (!m_targets_patrol.IsEmpty()) { m_target_patrol = m_targets_patrol[0]; }
}

void ANPC_Character::SetAssemblyPoint(const FString& AssemblyPoint)
{
	if (AssemblyPoint.IsEmpty()) return;

	TArray<AActor*> AssemblyPoints;
	UGameplayStatics::GetAllActorsWithTag(this, FName("AssemblyPoint"), AssemblyPoints);

	if (AssemblyPoints.IsEmpty()) return;

	AActor* actor = *AssemblyPoints.FindByPredicate([&](AActor* const Object)->bool {
			AFieldObject* field_object = Cast<AFieldObject>(Object);
			return field_object->GetUniqueName() == AssemblyPoint;
		});

	m_assembly_point = Cast<AAssemblyPoint>(actor);
	RegisteAssemblyPointMember();
}

void ANPC_Character::SetTimeValues(const float& WaitTimeMin, const float& WaitTimeMax, const float& PatrolTime, const float& KnowMissingTarget)
{
	m_time_wait_min			= WaitTimeMin;
	m_time_wait_max			= WaitTimeMax;
	m_time_patrol			= PatrolTime;
	m_sec_kwon_missing_target	= KnowMissingTarget;
}

void ANPC_Character::RegisteAssemblyPointMember()
{
	if (m_assembly_point) { m_assembly_point->RegisteAssemblyPointMember(this, m_request_callback); }
}

void ANPC_Character::StartPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(m_timer_patrol);
	if (m_request_callback.IsBound()) m_request_callback.Broadcast();
}

void ANPC_Character::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("NPC"));

	CHECK_INVALID_PTR(m_widget_healthbar)
	m_widget_healthbar->SetVisibility(false);
	m_widget_healthbar->InitStatusBar(m_attribute);
	RefreshHealthbar();

	m_controller_ai = Cast<AAIController>(GetController());

	CHECK_INVALID_PTR(m_pawn_sensing)
	m_pawn_sensing->OnSeePawn.AddDynamic(this, &ANPC_Character::PawnSeen);

	if (!ActorHasTag(FName("Boss"))) { RegisteAssemblyPointMember(); }

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)
	const bool load_complete = instance->IsLevelLoadComplete();
	load_complete ? GameStartAction() : instance->AddEventLevelLoadComplete(this, &ANPC_Character::GameStartAction);

	UCharacterMovementComponent* movement = GetCharacterMovement();
	CHECK_INVALID_PTR(movement)
}

void ANPC_Character::OnDeath(TWeakObjectPtr<UAnimMontage> Montage, const float& Duration)
{
	Super::OnDeath(Montage, Duration);

	SetBlindPawn(true);
	ClearAllTimer();

	m_action_state = EActionState_NPC::EASN_Unoccupied;

	if (m_assembly_point && m_target_assembly)
	{
		m_assembly_point->UnregistAssemblyPointMember(this);
		m_target_assembly->ResetPointObject();
		m_target_assembly = nullptr;
	}

	if (m_widget_healthbar) { m_widget_healthbar->SetVisibility(false); }
}

void ANPC_Character::GameStartAction()
{
	SetActorTransform(m_start_transform);
}

void ANPC_Character::Destroyed()
{
	Super::Destroyed();

	ClearAllTimer();
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
	CHECK_INVALID_PTR(anim_instance)
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

	CHECK_INVALID_PTR(m_widget_healthbar)
	m_widget_healthbar->SetVisibility(true);
	m_attribute->ReceiveDamage(damage);
	m_widget_healthbar->SetHealthPercent(m_attribute->GetHealthPercent());

	FName section_name = direction == EGameDirection::EGD_Front ? FName("TakeExecution_Front") : FName("TakeExecution_Back");
	PlayMontageSection(m_montage_take_execution, section_name);

	if (m_battle_pose == EBattlePose::EBP_Unoccupied) { m_actor_target = target_character; }
	ClearAllTimer();

	SetWeaponCollision(ECollisionEnabled::NoCollision);
}

const bool ANPC_Character::IsCurrentActionState(const FString& action) const
{
	if (action == "Attacking") { return m_action_state == EActionState_NPC::EASN_Attacking; }
	return false;
}

const bool ANPC_Character::IsGuardState() const
{
	FString battle_pose  = StaticEnum<EBattlePose>()->GetNameStringByValue(int64(m_battle_pose));
	FString action_state = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));

	UE_LOG(LogTemp, Warning, TEXT("IsGuardState - BattlePose : %s"), *battle_pose)
	UE_LOG(LogTemp, Warning, TEXT("IsGuardState - ActionState : %s"), *action_state)

	return m_action_state == EActionState_NPC::EASN_Confronting && m_battle_pose == EBattlePose::EBP_Defense;
}

void ANPC_Character::SetBlindPawn(const bool& is_blind)
{
	CHECK_INVALID_PTR(m_pawn_sensing)

	if (is_blind)	{ m_pawn_sensing->OnSeePawn.RemoveDynamic(this, &ANPC_Character::PawnSeen); }
	else		{ m_pawn_sensing->OnSeePawn.AddDynamic(this, &ANPC_Character::PawnSeen); }
}

bool ANPC_Character::EquipWeapon(const FName& socket_name, const EWeaponEquipHand& hand)
{
	UWorld* world = GetWorld();
	if (nullptr == world) return false;

	TObjectPtr<AWeapon_Actor>&  weapon	= hand == EWeaponEquipHand::EWEH_Right ? m_equiped_weapon_R : m_equiped_weapon_L;
	TSubclassOf<AWeapon_Actor> weapon_class	= hand == EWeaponEquipHand::EWEH_Right ? m_class_weapon_R : m_class_weapon_L;
	if (!weapon && weapon_class)
	{
		weapon = world->SpawnActor<AWeapon_Actor>(weapon_class);
		if (nullptr == weapon) return false;

		m_equip_state = EEquipState::EES_EquippedOneHandedWeapon;
	}

	weapon->Equip(GetMesh(), socket_name, this, this);

	return true;
}

const FString ANPC_Character::GetAssemblyPointLabel() const
{
	return m_assembly_point ? m_assembly_point->GetUniqueName() : FString();
}

const bool ANPC_Character::IsInSight(AActor* target)
{
	return m_pawn_sensing->CouldSeePawn(Cast<APawn>(target));
}

void ANPC_Character::InitAssemblyPointObject(AAssemblePointObject* const point_object)
{
	m_target_assembly = point_object;
	m_action_state	  = EActionState_NPC::EASN_Resting;
}

void ANPC_Character::MoveToAssemblePoint(AAssemblePointObject* const PointObject)
{
	ClearAllTimer();
	StopAllMontage();

	m_target_assembly = PointObject;
	m_action_state	  = EActionState_NPC::EASN_ReturningPointObject;

	FVector dest = PointObject->GetActionLocation();
	EGameDirection	direction = FCommonFunctions::FindDirection(this, dest);
	if      (direction == EGameDirection::EGD_Front) { MoveToLocation(dest, 0, false); }
	else if (direction != EGameDirection::EGD_None)  { StartTurn(direction); }
}

void ANPC_Character::MoveToPatrolPoint()
{
	m_action_state = EActionState_NPC::EASN_Patrolling;

	EGameDirection direction = FCommonFunctions::FindDirection(this, m_target_patrol->GetActorLocation());
	if      (direction == EGameDirection::EGD_Front) { MoveToLocation(target, -1.f); }
	else if (direction != EGameDirection::EGD_None)  { StartTurn(direction); }
}

const uint8 ANPC_Character::GetTargetDirectionFromNPC()
{
	return !m_actor_target ? 0 : (uint8)FCommonFunctions::FindDirection(this, m_actor_target->GetActorLocation());
}

void ANPC_Character::OnChangeVigilanceState(const EVigilanceState& state)
{
	m_vigilance_state = state;
}

void ANPC_Character::OnTurnEnd()
{
	if (nullptr == m_controller_ai) return;

	FVector direction = GetActorForwardVector();
	if (!direction.IsNearlyZero())
	{
		direction.Normalize();
	}
	direction *= GetMovementComponent()->GetMaxSpeed();

	m_controller_ai->SetControlRotation(direction.Rotation());
	m_controller_ai->StopMovement();
}

void ANPC_Character::OnTurnEndForAssembly()
{
	CHECK_INVALID_PTR(m_target_assembly)

	if (IsCantMoveState()) return;
	ANPC_Character::OnTurnEnd();

	if (m_action_state == EActionState_NPC::EASN_ReturningPointObject)
	{
		if (false == m_is_resting)
		{
			FVector	target = m_target_assembly->GetActionLocation();
			MoveToLocation(target, 0, true);
		}
		else
		{
			UCapsuleComponent* capsule = GetCapsuleComponent();
			if (nullptr == capsule) return;
			capsule->SetCanEverAffectNavigation(true);
			
			FString name = m_target_assembly->GetInteractionSectionName();
			PlayMontageSection(m_montage_resting, FName(name), -1.f);
		}
	}
}

void ANPC_Character::OnEndStunn()
{
	Super::OnEndStunn();

	SetWeaponCollision(ECollisionEnabled::QueryOnly);
	OnReactEnd();
}

void ANPC_Character::OnDeathCompleted()
{
	SetLifeSpan(3.f);
}

bool ANPC_Character::IsNeedChangeDeath(const EGameDirection& direction)
{
	if (m_attribute->IsAlive() || nullptr == m_montage_take_execution) return false;

	FName section_name;
	if (direction == EGameDirection::EGD_Front) { section_name = FName("TakeExecution_Death_Front"); m_death_pose = EDeathPose::EDP_Death_Front; }
	else					    { section_name = FName("TakeExecution_Death_Back");  m_death_pose = EDeathPose::EDP_Death_Back; }
	
	PlayMontageSection(m_montage_take_execution, section_name);
	OnDeath(m_montage_take_execution);

	return true;
}

void ANPC_Character::OnRestingSwitch()
{
	if (m_is_resting)
	{
		m_action_state = EActionState_NPC::EASN_Resting;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s OnRestingSwitch -> StartPatrol"), *GetUniqueName())
		StartPatrol();
	}
}

void ANPC_Character::OnRestingEnd(const FString& section_name)
{
	GetCapsuleComponent()->SetCanEverAffectNavigation(false);

	m_is_resting		= false;
	m_target_assembly	= nullptr;
	PlayMontageSection(m_montage_resting, FName(section_name));
}

void ANPC_Character::StartWatching()
{
	m_target_patrol = SelectPatrolTarget();
}

void ANPC_Character::StartPatrol()
{
	m_target_patrol = SelectPatrolTarget();
	if (!m_target_patrol) return;

	m_is_patrolling = true;
	m_is_resting	= false;

	GetWorldTimerManager().ClearTimer(m_timer_patrol);
	GetWorldTimerManager().SetTimer(m_timer_patrol, this, &ANPC_Character::StartPatrolTimer, m_time_patrol, false);
	
	MoveToPatrolPoint();
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
	if (nullptr != m_controller_ai)
	{
		m_controller_ai->StopMovement();
	}
}

void ANPC_Character::RefreshHealthbar(float DamageAmount)
{
	if (nullptr == m_attribute || nullptr == m_widget_healthbar) return;

	m_attribute->ReceiveDamage(DamageAmount);
	m_widget_healthbar->SetHealthPercent(m_attribute->GetHealthPercent());
}

void ANPC_Character::SaveCharacterData()
{
	if (false == m_attribute->IsAlive()) return;

	UERGameInstance* instance = GetGameInstance<UERGameInstance>();
	CHECK_INVALID_PTR(instance)

	instance->SaveNPC(this);
}

void ANPC_Character::AfterTargetDeath(TObjectPtr<AGameCharacter> Target)
{
}

const bool ANPC_Character::InAssmeblyPointAction()
{
	if (!IsAlive() || !m_target_assembly || m_vigilance_state == EVigilanceState::EVS_Vigilance) return false;

	FVector location = m_target_assembly->GetActorLocation();
	FVector action_location = UKismetMathLibrary::Quat_RotateVector(GetActorRotation().Quaternion(), location);

	m_is_resting	= true;
	m_is_patrolling	= false;

	EGameDirection direction = FCommonFunctions::FindDirection(this, m_assembly_point->GetActorLocation());
	if (direction == EGameDirection::EGD_Front)
	{
		UCapsuleComponent* capsule = GetCapsuleComponent();
		if (nullptr == capsule) return false;
		capsule->SetCanEverAffectNavigation(true);

		FString name = m_target_assembly->GetInteractionSectionName();
		PlayMontageSection(m_montage_resting, FName(name), -1.f);
	}
	else
	{
		StartTurn(direction);
	}

	return true;
}

const bool ANPC_Character::IsInTargetRange(AActor* target, double radius)
{
	if (nullptr == target) return false;

	const double distance = (target->GetActorLocation() - GetActorLocation()).Size();

	return distance <= radius;
}

void ANPC_Character::MoveToTarget(AActor* target, const float& accept_radius)
{
	if (nullptr == m_controller_ai || nullptr == target) return;

	FVector direction = target->GetActorLocation() - GetActorLocation();
	if (!direction.IsNearlyZero())
	{
		direction.Normalize();
	}
	direction *= GetMovementComponent()->GetMaxSpeed();

	m_controller_ai->SetControlRotation(direction.Rotation());
	EPathFollowingRequestResult::Type result = m_controller_ai->MoveToActor(target, accept_radius);
	if (result == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Move Failed"))
	}
}

void ANPC_Character::MoveToLocation(const FVector& dest, const float& acceptance_radius, const bool& stop_overlap)
{
	if (nullptr == m_controller_ai) { return; }

	FVector direction = dest - GetActorLocation();
	if (!direction.IsNearlyZero())
	{
		direction.Normalize();
	}
	direction *= GetMovementComponent()->GetMaxSpeed();

	m_controller_ai->SetControlRotation(direction.Rotation());
	EPathFollowingRequestResult::Type result = m_controller_ai->MoveToLocation(dest, acceptance_radius, stop_overlap);
	if (result == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Move Failed"))
	}
}

TObjectPtr<APatrolPoint> ANPC_Character::SelectPatrolTarget()
{
	if (m_targets_patrol.IsEmpty()) return nullptr;
	if (m_targets_patrol.Num() == 1) return m_target_patrol;

	TArray<TObjectPtr<APatrolPoint>> valid_target;
	for (const auto& point : m_targets_patrol)
	{
		if (point == m_target_patrol) continue;
		valid_target.AddUnique(point);
	}

	if (valid_target.IsEmpty()) return nullptr;

	int selected = FMath::RandRange(0, valid_target.Num() - 1);
	return valid_target[selected];
}

