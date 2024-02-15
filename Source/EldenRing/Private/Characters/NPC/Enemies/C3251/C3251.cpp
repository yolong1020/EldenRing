// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/Enemies/C3251/C3251.h"
#include "Characters/NPC/Enemies/C3251/C3251_Controller.h"
#include "Characters/NPC/Enemies/C3251/C3251_AI.h"
#include "Characters/NPC/Enemies/C3251/C3251_AnimInstance.h"
#include "Characters/GameCharacter_Interface.h"
#include "Characters/Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/SoundscapeManager.h"
#include "System/ObserverManager.h"
#include "HUD/BossHealthBar/BossHealthBarComponent.h"
#include "UI/Notify/EnemyFelled.h"
#include "Items/Weapons/MeleeAttackBody_Actor.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/CommonFunctions.h"
#include "Curves/CurveFloat.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

AC3251::AC3251() : m_is_second_phase(false)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AC3251_Controller::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	m_horse_capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HorseCapsule"));
	CHECK_INVALID_PTR(m_horse_capsule)
	m_horse_capsule->SetupAttachment(RootComponent);

	m_widget_bossbar = CreateDefaultSubobject<UBossHealthBarComponent>(TEXT("Health Bar Component"));
	CHECK_INVALID_PTR(m_widget_bossbar)

	m_latent.CallbackTarget = this;
	m_latent.ExecutionFunction = FName("OnEndAttack");
}

void AC3251::Destroyed()
{
	Super::Destroyed();

	if (m_attack_leg_L) { m_attack_leg_L->Destroy(); }
	if (m_attack_leg_R) { m_attack_leg_R->Destroy(); }
	if (m_attack_body)  { m_attack_body->Destroy(); }

	if (m_widget_bossbar) { m_widget_bossbar->HideBossHealthBar(); }
}

void AC3251::BeginPlay()
{
	Tags.Add(FName("Boss"));

	Super::BeginPlay();

	m_controller = Cast<AC3251_Controller>(GetController());
	CHECK_INVALID_SMART_PTR(m_controller)
	m_controller->GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	m_controller->GetBlackboardComponent()->SetValueAsBool(BBKEY_ACTING, false);
	m_controller->GetBlackboardComponent()->SetValueAsEnum(BBKEY_ACTIONTYPE, (uint8)ECombatAction_NPC::ECAN_End);

	EquipWeaponToStart();

	UClass* body_class = LoadClass<AMeleeAttackBody_Actor>(nullptr, TEXT("Blueprint'/Game/Blueprint/Items/Weapons/Body/BP_BodyWeapon_Actor.BP_BodyWeapon_Actor_C'"));
	CHECK_INVALID_PTR(body_class)

	UWorld* world =	GetWorld();
	CHECK_INVALID_PTR(world)
	m_attack_leg_L = world->SpawnActor<AMeleeAttackBody_Actor>(body_class);
	CHECK_INVALID_PTR(m_attack_leg_L)
	m_attack_leg_R = world->SpawnActor<AMeleeAttackBody_Actor>(body_class);
	CHECK_INVALID_PTR(m_attack_leg_R)
	m_attack_body = world->SpawnActor<AMeleeAttackBody_Actor>(body_class);
	CHECK_INVALID_PTR(m_attack_body)
	m_attack_leg_L->InitAttackBodyCollision(m_mesh, FName("Horse_L_HandSocket"), FVector::ZeroVector, FRotator(90, 0, 0), 30, 90, this, this);
	m_attack_leg_R->InitAttackBodyCollision(m_mesh, FName("Horse_R_HandSocket"), FVector::ZeroVector, FRotator(90, 0, 0), 30, 90, this, this);
	m_attack_body->InitAttackBodyCollision(m_mesh, FName("Horse_Spine2Socket"), FVector(-43, 0, 28), FRotator(90, 0, 0), 100, 180, this, this);

	m_horse_capsule->SetGenerateOverlapEvents(true);
	m_horse_capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_horse_capsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	m_horse_capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	m_horse_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	m_horse_capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);

	m_sphere_execution_front->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_sphere_execution_back->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_sphere_interaction->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_sphere_parry->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_actions = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/Blueprint/Data/ActionList/C3251/DT_C3251_CombatAction.DT_C3251_CombatAction'"), nullptr, LOAD_None, nullptr);
	CHECK_INVALID_PTR(m_actions)

	TMap<FName, FNPC_CombatActionData*> actions;
	TArray<FNPC_CombatActionData*> all_action;
	m_actions->GetAllRows("Failed to get all actions", all_action);
	for (auto action : all_action)
	{
		if (action->CombatActionType == ECombatAction_NPC::ECAN_SingleAttack &&
			action->DerivedAttack.IsEmpty() == false) { actions.Add(action->ActionID, action); }
	}
	for (uint8 i = (uint8)EGameDirection::EGD_Front; i <= (uint8)EGameDirection::EGD_Right; ++i)
	{
		UAnimMontage* montage = nullptr;
		switch (EGameDirection(i))
		{
			case EGameDirection::EGD_Front: montage = m_montage_single_front; break;
			case EGameDirection::EGD_Left:  montage = m_montage_single_right; break;
			case EGameDirection::EGD_Right: montage = m_montage_single_left;  break;
		}

		for (const auto& section : montage->CompositeSections)
		{
			if (false == actions.Contains(section.SectionName)) continue;
			m_combo_singles[i - 1].Add(actions[section.SectionName]);
		}
	}

	CHECK_INVALID_PTR(m_widget_bossbar)
	m_widget_bossbar->InitStatusBar(FText::FromString(TEXT("트리 가드")), m_attribute);

	m_action_combat = ECombatAction_NPC::ECAN_End;
	m_equip_state   = EEquipState::EES_EquippedOneHandedWeapon;
	m_is_canceled   = false;

	UGameInstance* instance = GetGameInstance();
	CHECK_INVALID_PTR(instance)
	m_sound_mgr = instance->GetSubsystem<USoundscapeManager>();
	CHECK_INVALID_SMART_PTR(m_sound_mgr)
	m_observer_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_SMART_PTR(m_observer_mgr)
	m_observer_mgr->RegistObserver<AC3251>(EObserverEventType::EOET_SaveData, this, &AC3251::SaveCharacterData);

	FOnTimelineFloat tl_rotate;
	tl_rotate.BindUFunction(this, FName("OnRotateMoving"));
	m_tl_rotate.AddInterpFloat(m_curve_rotate, tl_rotate);
}

void AC3251::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_tl_rotate.TickTimeline(DeltaTime);
}

void AC3251::GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight)
{
	CHECK_INVALID_PTR(m_attribute)
	CHECK_INVALID_PTR(m_widget_healthbar)

	bool is_alive = m_attribute->IsAlive();

	if (is_alive)
	{
		HitReact(Dir, AttackWeight);

		if (0.5 >= m_attribute->GetHealthPercent() && false == m_is_second_phase)
		{
			m_is_second_phase = true;
			m_sound_mgr->ChangePhaseBGM();
		}
	}
	else
	{
		if (EDeathPose::EDP_Alive != m_death_pose) return;
		AutoPossessAI = EAutoPossessAI::Disabled;

		StopAllMontage();
	
		float play_rate = 0.5;
		PlayMontageSection(m_montage_death, FName("Death"), play_rate);
		float duration = (m_montage_death->GetSectionLength(0) - 1) / play_rate;

		OnDeath(m_montage_death, duration);

		m_widget_bossbar->HideBossHealthBar();
	}

	if (m_sound_hit)    { UGameplayStatics::PlaySoundAtLocation(this, m_sound_hit, ImpactPoint); }
	if (m_particle_hit) { UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_particle_hit, ImpactPoint); }
}

void AC3251::SetWeaponCollision(ECollisionEnabled::Type type)
{
	IWeapon_Interface* weapon_interface = Cast<IWeapon_Interface>(m_equiped_weapon_R);
	CHECK_INVALID_PTR(weapon_interface)
	weapon_interface->SetWeaponCollision(type);
}

float AC3251::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}

void AC3251::EquipWeaponToStart()
{
	if (m_class_weapon_R) { EquipWeapon("R_HartbertSocket", EWeaponEquipHand::EWEH_Right); }
	if (m_class_weapon_L) { EquipWeapon("L_ShiledSocket", EWeaponEquipHand::EWEH_Left); }
}

void AC3251::TrackingTarget()
{
	m_action_state = EActionState_NPC::EASN_Chasing;
	MoveToTarget(m_actor_target);
}

void AC3251::OnMoveCompleted(const FPathFollowingResult& Result)
{
}

void AC3251::OnBattle()
{
	if (m_actor_target)
	{
		TrackingTarget();
		m_animation_callback.ExecuteIfBound();

		UE_LOG(LogTemp, Warning, TEXT("On Battle"))
		m_controller->GetBlackboardComponent()->SetValueAsBool(BBKEY_ACTING, false);
		m_controller->GetBlackboardComponent()->SetValueAsEnum(BBKEY_ACTIONTYPE, (uint8)ECombatAction_NPC::ECAN_DashAttack);
	}
}

void AC3251::OnEndAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Try End Attack Callback"))
	m_animation_callback.ExecuteIfBound();
}

void AC3251::OnEndMove()
{
	m_animation_callback.ExecuteIfBound();
}

void AC3251::OnInputCombo()
{
	if (false == m_controller->GetBlackboardComponent()->GetValueAsBool(BBKEY_COMBOBOUND) ||
	    false == HasDerivedAttack() ||
	    EGameDirection::EGD_Left != FCommonFunctions::FindDirection(this, m_actor_target->GetActorLocation())) return;

	m_animation_callback.ExecuteIfBound();
}

void AC3251::SetBodyCollision(const ECollisionEnabled::Type& Type)
{
	IWeapon_Interface* weapon_interface = Cast<IWeapon_Interface>(m_attack_body);
	CHECK_INVALID_PTR(weapon_interface)
	weapon_interface->SetWeaponCollision(Type);
	
	if (Type == ECollisionEnabled::NoCollision) { m_ignore_target.Empty(); }
}

void AC3251::SetLegsCollision(const ECollisionEnabled::Type& Type)
{
	IWeapon_Interface* l_interface = Cast<IWeapon_Interface>(m_attack_leg_L);
	CHECK_INVALID_PTR(l_interface)
	l_interface->SetWeaponCollision(Type);

	IWeapon_Interface* r_interface = Cast<IWeapon_Interface>(m_attack_leg_R);
	CHECK_INVALID_PTR(r_interface)
	r_interface->SetWeaponCollision(Type);

	if (Type == ECollisionEnabled::NoCollision) { m_ignore_target.Empty(); }
}

void AC3251::SetShieldCollision(const ECollisionEnabled::Type Type)
{
	IWeapon_Interface* weapon_interface = Cast<IWeapon_Interface>(m_equiped_weapon_L);
	CHECK_INVALID_PTR(weapon_interface)
	weapon_interface->SetWeaponCollision(Type);
}

void AC3251::OnShowDirecting()
{
	UClass* felled_class = LoadClass<UEnemyFelled>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Notify/WBP_EnemyFelled.WBP_EnemyFelled_C'"));
	CHECK_INVALID_PTR(felled_class)
	UEnemyFelled* felled_notify = CreateWidget<UEnemyFelled>(GetWorld(), felled_class);
	CHECK_INVALID_PTR(felled_notify)

	felled_notify->StartDirecting();
}

void AC3251::StartRotateMoving()
{
	m_tl_rotate.PlayFromStart();
}

void AC3251::OnRotateMoving(float Delta)
{
	CHECK_INVALID_PTR(m_actor_target)
	FRotator goal = (m_actor_target->GetActorLocation() - GetActorLocation()).Rotation();

	FQuat calc = FQuat::Slerp(GetActorRotation().Quaternion(), goal.Quaternion(), 0.1f * Delta);
	SetActorRotation(calc);

	if (goal.Quaternion() == calc) { m_tl_rotate.Stop(); }
}

void AC3251::HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight)
{
	PlayMontageSection(m_hit_react, FName("HitReact"));
}

void AC3251::PawnSeen(APawn* seen_pawn)
{
	AGameCharacter* target = Cast<AGameCharacter>(seen_pawn);
	if (!target) return;

	if (!m_actor_target && seen_pawn->ActorHasTag(FName("Player")))
	{
		m_actor_target = target;
		m_observer_mgr->RegistDeathObserver<AC3251>(m_actor_target, this, &AC3251::AfterTargetDeath);

		m_controller->GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, target);
		m_controller->GetBlackboardComponent()->SetValueAsBool(BBKEY_ACTING, true);

		StartCombat();
	}
}

void AC3251::OnMontageStarted(UAnimMontage* Montage)
{
}

void AC3251::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!m_attribute->IsAlive()) return;

	FString name = Montage->GetName();
	if (m_playing_montages.Contains(name)) { m_playing_montages.Remove(name); }

	if (Montage == m_hit_react || !m_is_canceled) return;

	UE_LOG(LogTemp, Warning, TEXT("In Cancel : %s"), *name)

	m_is_canceled = false;
	m_animation_callback.ExecuteIfBound();
}

void AC3251::StartCombat()
{
	if (m_vigilance_state == EVigilanceState::EVS_Vigilance) return;

	m_widget_bossbar->ShowBossHealthBar();
	PlayMontageSection(m_montage_on_battle, FName("3013"));
	m_playing_montages.Add(m_montage_on_battle->GetName());

	AWeapon_Actor* weapon = m_equiped_weapon_R;
	IWeapon_Interface* weapon_interface = Cast<IWeapon_Interface>(weapon);
	CHECK_INVALID_PTR(weapon_interface)
	weapon_interface->ForeceSetAttackWeight(EAttackWeight::EAW_Medium);

	USoundBase* boss_bgm = LoadObject<USoundBase>(nullptr, TEXT("MetaSoundSource'/Game/EldenRing/Sounds/BackgroundMusic/MS_TreeGuard.MS_TreeGuard'"));
	CHECK_INVALID_PTR(boss_bgm)
	m_sound_mgr->PlayBossBGM(boss_bgm, 1, 1);
}

void AC3251::TryDashAttack()
{
	m_controller_ai->StopMovement();
	m_action_combat = ECombatAction_NPC::ECAN_DashAttack;

	bool is_phase_attack = m_is_second_phase ? ((FMath::Rand() % 100 < 60) ? true : false) : false;
	UAnimMontage* montage = is_phase_attack ? m_montage_dash_attack_phase2 : m_montage_dash_attack;
	int32 count = montage->GetNumSections();
	int idx = (count == 1) ? 0 : FMath::RandRange(0, count- 1);

	prv_attack_section = montage->GetSectionName(idx);
	PlayMontageSection(montage, prv_attack_section);
	m_playing_montages.Add(montage->GetName());

	FNPC_CombatActionData* action_info = m_actions->FindRow<FNPC_CombatActionData>(prv_attack_section, prv_attack_section.ToString());
	CHECK_INVALID_PTR(action_info)

	AWeapon_Actor* weapon = action_info->WeaponType == EWeaponType::EWT_Halberd ? m_equiped_weapon_R : m_equiped_weapon_L;
	IWeapon_Interface* weapon_interface = Cast<IWeapon_Interface>(weapon);
	CHECK_INVALID_PTR(weapon_interface)
	weapon_interface->ForeceSetAttackWeight(action_info->AttackWeight);
}

void AC3251::TrySingleAttack()
{
	m_controller_ai->StopMovement();

	UAnimMontage* montage = nullptr;
	EGameDirection direction = FCommonFunctions::FindDirection(this, m_actor_target->GetActorLocation());
	switch (direction)
	{
		case EGameDirection::EGD_Front: montage = m_is_second_phase ? ((FMath::Rand() % 100 < 60) ? m_montage_single_front_phase2 : m_montage_single_front) : m_montage_single_front; break;
		case EGameDirection::EGD_Left:  montage = m_montage_single_right; break;
		case EGameDirection::EGD_Right: montage = m_is_second_phase ? ((FMath::Rand() % 100 < 60) ? m_montage_single_left_phase2 : m_montage_single_left) : m_montage_single_left;  break;
		default: { CancelAction(); return; }
	}

	UBlackboardComponent* const blackboard = m_controller->GetBlackboardComponent();
	CHECK_INVALID_PTR(blackboard)

	bool is_in_single = blackboard->GetValueAsBool(BBKEY_SINGLEBOUND);
	bool is_in_combo  = blackboard->GetValueAsBool(BBKEY_COMBOBOUND);

	TArray<FNPC_CombatActionData*> enable_actions;
	if (blackboard->GetValueAsBool(BBKEY_SINGLECOMBO))
	{
		for (FNPC_CombatActionData* const data : m_combo_singles[(uint8)direction - 1])
		{
			if ((data->ActionBound == ECombatBound::ECB_Confront && is_in_single) ||
				(data->ActionBound == ECombatBound::ECB_Attack && is_in_combo))
				enable_actions.Add(data);
		}
	}
	else
	{
		for (const auto& section : montage->CompositeSections)
		{
			FNPC_CombatActionData* action_info = m_actions->FindRow<FNPC_CombatActionData>(section.SectionName, section.SectionName.ToString());
			if (!action_info) continue;

			if ((action_info->ActionBound == ECombatBound::ECB_Confront && is_in_single) || 
				(action_info->ActionBound == ECombatBound::ECB_Attack && is_in_combo)) 
				enable_actions.Add(action_info);
		}
	}

	if (enable_actions.IsEmpty())
	{
		CancelAction();
		return;
	}
	
	int32 count = enable_actions.Num();
	int idx = (count == 1) ? 0 : FMath::RandRange(0, count - 1);

	for (const auto& section : montage->CompositeSections)
	{
		if (section.SectionName == enable_actions[idx]->ActionID) break;
	}

	prv_attack_section = enable_actions[idx]->ActionID;
	m_action_combat = enable_actions[idx]->CombatActionType;

	PlayMontageSection(montage, prv_attack_section);
	m_playing_montages.Add(montage->GetName());

	AWeapon_Actor* weapon = enable_actions[idx]->WeaponType == EWeaponType::EWT_Halberd ? m_equiped_weapon_R : m_equiped_weapon_L;
	IWeapon_Interface* weapon_interface = Cast<IWeapon_Interface>(weapon);
	CHECK_INVALID_PTR(weapon_interface)
	weapon_interface->ForeceSetAttackWeight(enable_actions[idx]->AttackWeight);
}

void AC3251::TryComboAttack()
{
	m_controller_ai->StopMovement();

	EGameDirection		direction	= FCommonFunctions::FindDirection(this, m_actor_target->GetActorLocation());
	FNPC_CombatActionData*	derived_attack	= m_actions->FindRow<FNPC_CombatActionData>(prv_attack_section, prv_attack_section.ToString());
	CHECK_INVALID_PTR(derived_attack)

	int32 count = derived_attack->DerivedAttack.Num();
	int idx = (count == 1) ? 0 : FMath::RandRange(0, count - 1);

	FName section = derived_attack->DerivedAttack[idx];
	FNPC_CombatActionData* cur_attack = m_actions->FindRow<FNPC_CombatActionData>(section, section.ToString());
	CHECK_INVALID_PTR(cur_attack)

	m_action_combat	   = cur_attack->CombatActionType;
	prv_attack_section = section;

	UAnimMontage* montage = nullptr;
	switch (direction)
	{
		case EGameDirection::EGD_Back: 
			if (derived_attack->ActionDirection != direction) { CancelAction(); return; }
			else { montage = m_montage_combo_back;  break; }
		case EGameDirection::EGD_Left: 
			if (derived_attack->ActionDirection != direction) { CancelAction(); return; }
			else { montage = m_montage_combo_right;  break; }
		case EGameDirection::EGD_Front:
		case EGameDirection::EGD_Right: CancelAction(); return;
	}
	 	
	CHECK_INVALID_PTR(montage)
	PlayMontageSection(montage, section);
	m_playing_montages.Add(montage->GetName());

	AWeapon_Actor* weapon = derived_attack->WeaponType == EWeaponType::EWT_Halberd ? m_equiped_weapon_R : m_equiped_weapon_L;
	IWeapon_Interface* weapon_interface = Cast<IWeapon_Interface>(weapon);
	CHECK_INVALID_PTR(weapon_interface)
	weapon_interface->ForeceSetAttackWeight(derived_attack->AttackWeight);
}

void AC3251::TryMoveAction()
{
	m_controller_ai->StopMovement();

	int32 count = m_montage_move_action->GetNumSections();
	int idx = FMath::Rand() % count;

	m_action_combat		= ECombatAction_NPC::ECAN_MoveAction;
	prv_attack_section	= m_montage_move_action->GetSectionName(idx);
	PlayMontageSection(m_montage_move_action, prv_attack_section);
	m_playing_montages.Add(m_montage_move_action->GetName());
}

const bool AC3251::HasDerivedAttack()
{
	if (prv_attack_section == "") return false;

	FNPC_CombatActionData* prv_action = m_actions->FindRow<FNPC_CombatActionData>(prv_attack_section, prv_attack_section.ToString());
	if (!prv_action) return false;
	
	return !prv_action->DerivedAttack.IsEmpty();
}

void AC3251::GetOffsetDegreeAndInterptSpeed(float& Degree, float& Speed)
{
	if (prv_attack_section == "") return;

	FNPC_CombatActionData* cur_attack = m_actions->FindRow<FNPC_CombatActionData>(prv_attack_section, prv_attack_section.ToString());
	if (!cur_attack) return;

	Degree = cur_attack->OffsetDegree;
	Speed  = cur_attack->InterptSpeed;
}

void AC3251::FinishFadeOut(UNiagaraComponent* PSystem)
{
	Super::FinishFadeOut(PSystem);

	UninitAndDestroy();
	m_sound_mgr->ReplayStageBGM();
}

void AC3251::AfterTargetDeath(TObjectPtr<class AGameCharacter> Target)
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
		callback.UUID 			= FGuid::NewGuid().A;
		callback.CallbackTarget 	= this;
		callback.Linkage 		= 0;
		callback.ExecutionFunction 	= FName("IdleMode");

		float duration = (float)FMath::RandRange(1.f, WaitAfterPlayerDeath);
		UKismetSystemLibrary::Delay(this, duration, callback);
	}
}

void AC3251::GameStartAction()
{
	Super::GameStartAction();
}

void AC3251::CancelAction()
{
	UE_LOG(LogTemp, Warning, TEXT("In CancelAction"))
	m_is_canceled = true;
	if (m_playing_montages.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing Montage is Empty"))
		m_is_canceled = false;
		m_animation_callback.ExecuteIfBound();
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("Current Playing Actions : %d"), m_playing_montages.Num())
	}
}

void AC3251::IdleMode()
{
	m_action_state = EActionState_NPC::EASN_Unoccupied;

	m_controller->GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	UE_LOG(LogTemp, Warning, TEXT("In Idle"))
	m_controller->GetBlackboardComponent()->SetValueAsBool(BBKEY_ACTING, false);
}
