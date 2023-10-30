// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Players/C0000.h"
#include "Characters/Players/C0000_AnimInstance.h"
#include "Characters/GameCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "HUD/Status/GameHUD_Actor.h"
#include "HUD/Status/GameHUD.h"
#include "HUD/Inventory/Inventory.h"
#include "HUD/Inventory/InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Item.h"
#include "Items/ItemData.h"
#include "Items/ItemObject.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Items/Weapons/MeleeAttack_Actor.h"
#include "Items/Weapons/Shield_Actor.h"
#include "Items/Equipment/Equipment.h"
#include "Engine/SkeletalMesh.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/SkeletalMeshLODModel.h"
#include "Perception/PawnSensingComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimNode_SequencePlayer.h"
#include "Animation/AnimNode_AssetPlayerBase.h"
#include "TimerManager.h"
#include "Camera/ExecutionCameraShake.h"
#include "../DebugMacros.h"

AC0000::AC0000()
{
 	PrimaryActorTick.bCanEverTick	= true;

	bUseControllerRotationYaw	= false;
	bUseControllerRotationPitch	= false;
	bUseControllerRotationRoll	= false;

	m_equiped_item.SetNum((int32)EEquipmentType::EET_Legs);

	//	Movement
	m_movement_component = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	CHECK_INVALID(m_movement_component)
	
	m_movement_component->bOrientRotationToMovement = true;
	m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;
	m_movement_component->RotationRate = FRotator(0.f, 400.f, 0.f);

	USkeletalMeshComponent* skeletal_mesh = GetMesh();
	CHECK_INVALID(skeletal_mesh)

	skeletal_mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	skeletal_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	skeletal_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	skeletal_mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	skeletal_mesh->SetGenerateOverlapEvents(true);

	//	Spring Arm
	m_spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_spring_arm->SetupAttachment(RootComponent);

	//	Camera
	m_view_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	m_view_camera->SetupAttachment(m_spring_arm);

	//	Inventory
	m_inventory_component = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	//	Sphere Detect
	m_sphere_detect = CreateDefaultSubobject<USphereComponent>(TEXT("SphereDetect"));
	m_sphere_detect->SetupAttachment(RootComponent);
}

void AC0000::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Player"));

	m_overlapping_item	= nullptr;
	m_enable_attack		= true;
	m_enable_attack_short	= false;
	m_enable_sprint_turn	= false;
	m_is_not_damage_mod	= false;
	m_total_potion		= 3;

	m_player_controller = Cast<APlayerController>(Controller);
	CHECK_INVALID(m_player_controller)

	m_spring_arm_length = m_spring_arm->TargetArmLength;

	CHECK_INVALID(m_sphere_detect)
	m_sphere_detect->OnComponentBeginOverlap.AddDynamic(this, &AC0000::OnDetectOverlap);
	m_sphere_detect->OnComponentEndOverlap.AddDynamic(this, &AC0000::OnDetectEnd);
	m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//	HUD
	AGameHUD_Actor* overlay_actor = Cast<AGameHUD_Actor>(m_player_controller->GetHUD());
	CHECK_INVALID(overlay_actor)
	m_hud_overlay = overlay_actor->GetOverlayHUD();
	CHECK_INVALID(m_hud_overlay)
	m_hud_overlay->InitialStatusBar(m_attribute);

	//	Camera Timeline
	FOnTimelineFloat timeline_focus_callback;
	timeline_focus_callback.BindUFunction(this, FName("OnCameraFocusRotate"));
	m_timeline_camera_focus.AddInterpFloat(m_curve_camera_focus, timeline_focus_callback);

	FOnTimelineFloat timeline_execution_front;
	timeline_execution_front.BindUFunction(this, FName("OnCameraExecutionFront"));
	m_timeline_camera_execution_front.AddInterpFloat(m_curve_camera_execution_front, timeline_execution_front);

	FOnTimelineFloat timeline_execution_back;
	timeline_execution_back.BindUFunction(this, FName("OnCameraExecutionBack"));
	m_timeline_camera_execution_back.AddInterpFloat(m_curve_camera_execution_back, timeline_execution_back);

	//	Default Items
	UWorld* world = GetWorld();
	CHECK_INVALID(world)

	AWeapon_Actor* weapon = world->SpawnActor<AWeapon_Actor>(m_class_weapon_R);
	CHECK_INVALID(weapon)

	m_inventory_component->TryAddItemAndEquip(weapon->GetItemObject());
	m_inventory_component->RefreshGoldAmount(m_total_gold);
	weapon->Destroy();
	m_equip_state = EEquipState::EES_EquippedOneHandedWeapon;

	m_hud_overlay->SetQuickSlotItem(EQuickSlotType::EQST_Bottom, nullptr, EPotionSizeType::EPST_Full);
}

void AC0000::Tick(float DeltaTime)
{
	m_timeline_camera_focus.TickTimeline(DeltaTime);
	m_timeline_camera_execution_front.TickTimeline(DeltaTime);
	m_timeline_camera_execution_back.TickTimeline(DeltaTime);
}

void AC0000::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	m_jump_state = EJumpState::EJS_Landing;

	UAnimMontage*	montage		= m_montage_land;
	float		speed_cur	= m_movement_component->Velocity.Size2D();
	FString		section_name	= (150.f <= speed_cur) ? FString("Land_Front") : FString("Land_Standing");

	if (ELockOnState::ELOS_LockOn == m_lock_on_state)
	{
		float scale = 0;
		if (IsInputKey(FName("MoveForward"), scale, true))
		{
			if (scale < 0)		section_name = FString("Land_Back");
		}
		else if (IsInputKey(FName("MoveSide"), scale, true))
		{
			if (scale > 0)		section_name = FString("Land_Right");
			else if (scale < 0)	section_name = FString("Land_Left");
		}
	}

	if (300.f <= speed_cur)
	{
		section_name += FString("_Jog");
		montage = m_montage_land_jog;
	}
	UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
	if (nullptr == anim_instance) return;

	m_movement_component->SetMovementMode(EMovementMode::MOVE_Walking);
	anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	PlayMontage(montage, FName(section_name));
}

void AC0000::HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight)
{
	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID(anim_instance)

	bool		is_need_blend		= true;
	FString		hit_section_postfix	= "";
	int32		selected_index		= 0;

	UAnimMontage*	montage_with_direction	= nullptr;
	UAnimMontage*	montage_hit 		= nullptr;

	switch (attack_weight)
	{
		case EAttackWeight::EAW_Small:
		{
			hit_section_postfix		= FString("Small");
		}break;
		case EAttackWeight::EAW_Medium:
		{
			hit_section_postfix		= FString("Medium");
			montage_hit			= m_montage_hit_medium;
		} break;
		case EAttackWeight::EAW_Heavy:
		{
			hit_section_postfix		= FString("Heavy");
			montage_with_direction		= nullptr;
		} break;
		case EAttackWeight::EAW_ExtraHeavy:
		{
			hit_section_postfix		= FString("ExtraHeavy");
			montage_with_direction		= nullptr;
		} break;
		case EAttackWeight::EAW_UltraHeavy:
		{
			hit_section_postfix		= FString("UltraHeavy");
			montage_with_direction		= nullptr;
			montage_hit			= m_montage_hit_ultra;
		} break;
	}
	CHECK_INVALID(montage_hit)

	switch (hit_direction)
	{
		case EGameDirection::EGD_Front: hit_section_postfix += FString("_Front"); break;
		case EGameDirection::EGD_Back:	hit_section_postfix += FString("_Back");  break;
		case EGameDirection::EGD_Left:	hit_section_postfix += FString("_Left");  break;
		case EGameDirection::EGD_Right: hit_section_postfix += FString("_Right"); break;
		default: return;
	}

	if (attack_weight != EAttackWeight::EAW_UltraHeavy)
	{
		selected_index	=  FMath::RandRange(1, montage_hit->GetNumSections() / 4);
		hit_section_postfix += (FString("_0") + FString::FromInt(selected_index));
	}

	PlayMontage(montage_hit, FName(hit_section_postfix));
	anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);

	bool is_already_clear = false;
	for (FAnimMontageInstance* instance : anim_instance->MontageInstances)
	{
		if (false   == instance->bPlaying ||
		    nullptr == instance->Montage ||
		    false   == instance->Montage->GetFName().ToString().Contains("AM_HitReact")) continue;

		int32 index = instance->Montage->GetSectionIndex(FName(hit_section_postfix));
		if (0 > index) continue;
		FCompositeSection section = instance->Montage->GetAnimCompositeSection(index);
		int32 segment_index 	  = section.GetSegmentIndex();

		if (false == instance->Montage->SlotAnimTracks.IsValidIndex(0)) continue;

		FSlotAnimationTrack& default_slot = instance->Montage->SlotAnimTracks[0];
		if (false == default_slot.AnimTrack.AnimSegments.IsValidIndex(segment_index)) continue;

		FAnimSegment segment = default_slot.AnimTrack.AnimSegments[segment_index];
		float	     length  = segment.GetLength();

		bool is_need_clear = (instance->Montage == montage_hit);
		if (false == is_already_clear)
		{
			GetWorldTimerManager().ClearTimer(m_timer_hit_react);
			GetWorldTimerManager().SetTimer(m_timer_hit_react, this, &AC0000::OnReactEnd, length - 0.2);
			is_already_clear = true;
		}
	}

	m_action_state = EActionState::EAS_HitReact;
}

void AC0000::OnReactEnd()
{
	CHECK_INVALID(m_movement_component)
	m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;

	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID(anim_instance)
	anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

	m_enable_attack = true;
	m_action_state	= EActionState::EAS_Unoccupied;
}

bool AC0000::IsInputPossible()
{
	return	m_action_state != EActionState::EAS_Equipping &&
			m_action_state != EActionState::EAS_Execution &&
			m_action_state != EActionState::EAS_Dodgeing &&
			m_action_state != EActionState::EAS_Turning &&
			m_action_state != EActionState::EAS_HitReact &&
			m_action_state != EActionState::EAS_BlockReact &&
			m_action_state != EActionState::EAS_GuardReact &&
			m_action_state != EActionState::EAS_Knockbacking &&
			m_action_state != EActionState::EAS_Parry;
}

void AC0000::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AC0000::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveSide"), this, &AC0000::MoveSide);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AC0000::LookUp);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AC0000::Turn);
	PlayerInputComponent->BindAxis(FName("LockOnTargetChange"), this, &AC0000::LockOnTargetChange);
	
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AC0000::Attack);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Released, this, &AC0000::ShortAttack);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AC0000::StartJump);
	PlayerInputComponent->BindAction(FName("Interaction"), IE_Pressed, this, &AC0000::Interaction);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AC0000::Equip);
	PlayerInputComponent->BindAction(FName("LockOn"), IE_Pressed, this, &AC0000::LockOn);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AC0000::Dodge);
	PlayerInputComponent->BindAction(FName("Inventory"), IE_Pressed, this, &AC0000::Inventory);
	PlayerInputComponent->BindAction(FName("Guard"), IE_Pressed, this, &AC0000::Guard);
	PlayerInputComponent->BindAction(FName("Guard"), IE_Released, this, &AC0000::Guard);
	PlayerInputComponent->BindAction(FName("Parry"), IE_Pressed, this, &AC0000::Parry);
	PlayerInputComponent->BindAction(FName("Consume"), IE_Pressed, this, &AC0000::Consume);
}

void AC0000::GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight)
{
	CHECK_INVALID(m_attribute)

	if (m_attribute->IsAlive())
	{
		EGameDirection hit_direction = FindDirection(this, ImpactPoint);

		StopAllMontage();
		HitReact(hit_direction, attack_weight);
	}
	else
	{
		if (EDeathPose::EDP_Alive != m_death_pose) { return; }
	}

	if (nullptr != m_sound_hit) { UGameplayStatics::PlaySoundAtLocation(this, m_sound_hit, ImpactPoint); }
	if (nullptr != m_particle_hit)
	{
		auto world = GetWorld();
		CHECK_INVALID(world)

		UGameplayStatics::SpawnEmitterAtLocation(world, m_particle_hit, ImpactPoint);
	}
}

float AC0000::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	m_attribute->ReceiveDamage(DamageAmount);
	m_hud_overlay->RefreshStatBar(EOverlayStatType::EOST_Health, m_attribute->GetHealthPercent(), false);

	if (nullptr == EventInstigator) return DamageAmount;

	APawn* pawn = EventInstigator->GetPawn();
	if (nullptr == pawn) return DamageAmount;

	return DamageAmount;
}

void AC0000::OnAttackDefended(const EAttackWeight& attack_weight)
{
	FName section_name;

	switch (attack_weight)
	{
		case EAttackWeight::EAW_Medium:		section_name = FName("Guard_Medium");	  break;
		case EAttackWeight::EAW_Heavy:		section_name = FName("Guard_Heavy");	  break;
		case EAttackWeight::EAW_ExtraHeavy: 	section_name = FName("Guard_ExtraHeavy"); break;
		case EAttackWeight::EAW_UltraHeavy:	section_name = FName("Guard_ExtraHeavy"); break;
		case EAttackWeight::EAW_Small:		section_name = FName("Guard_Medium");	  break;
		case EAttackWeight::EAW_None: return;
	}

	m_action_state = EActionState::EAS_Knockbacking;
	PlayMontage(m_montage_guard, section_name);
}

void AC0000::OnAttackBlocked(const EAttackWeight& attack_weight)
{
	FName section_name = FName("Blocked_Strong");

	m_action_state = EActionState::EAS_BlockReact;

	PlayMontage(m_montage_blocked, section_name);
	SetWeaponCollision(ECollisionEnabled::Type::NoCollision);
}

void AC0000::OnCameraShakeIn()
{
	UGameplayStatics::PlayWorldCameraShake(this, m_class_camera_shake_execution_in, m_actor_target->GetActorLocation(), 0, 500);
}

void AC0000::OnCameraShakeOut()
{
	UGameplayStatics::PlayWorldCameraShake(this, m_class_camera_shake_execution_out, m_actor_target->GetActorLocation(), 0, 500);
}

void AC0000::OnSuccessConsume()
{
	m_attribute->RecoveryHealth(40);
	m_hud_overlay->RefreshStatBar(EOverlayStatType::EOST_Health, m_attribute->GetHealthPercent(), true);

	PlayMontage(m_montage_potion_consume, FName("Consume_End"));
}

void AC0000::OnEndConsume()
{
	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID(anim_instance)
	anim_instance->SetBlendPerBone(false);

	m_action_state = EActionState::EAS_Unoccupied;
}

void AC0000::OnUseStamina(const int32& amount_stamina)
{
	m_attribute->ConsumeStamina(amount_stamina);
	m_hud_overlay->RefreshStatBar(EOverlayStatType::EOST_Stamina, m_attribute->GetStaminaPercent(), false);
}

bool AC0000::IsCurrentActionState(const FString& action)
{
	FString action_state = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));

	if (action == "Attacking") { return m_action_state == EActionState::EAS_Attacking; }
	return false;
}

bool AC0000::IsGuardState()
{
	return m_action_state == EActionState::EAS_Guarding;
}

void AC0000::MoveForward(float value)
{
	if (nullptr == Controller || 0 == value) return;
	Movement(EAxis::X, value);
}

void AC0000::MoveSide(float value)
{
	if (nullptr == Controller || 0 == value) return;
	Movement(EAxis::Y, value);
}

void AC0000::Movement(const EAxis::Type axis, const float& value)
{
	if (false == IsInputPossible()) return;

	if (EActionState::EAS_Attacking != m_action_state)
	{
		bool is_key_down = m_player_controller->IsInputKeyDown(EKeys::RightMouseButton);
		if (false == is_key_down && m_action_state != EActionState::EAS_Consume) { m_action_state = EActionState::EAS_Unoccupied; }

		m_is_sprint = m_player_controller->IsInputKeyDown(EKeys::LeftShift) && (0 < m_attribute->GetStaminaCurrent());

		if (m_lock_on_state == ELockOnState::ELOS_LockOn)
		{
			CHECK_INVALID(m_actor_target)
		
			FVector direction		= m_actor_target->GetActorLocation() - GetActorLocation();
			FVector direction_rotated	= FRotationMatrix(direction.Rotation()).GetUnitAxis(axis);

			m_movement_component->bOrientRotationToMovement = m_is_sprint;

			AddMovementInput(direction_rotated, value);
		}
		else
		{
			const FRotator control_rotation = GetControlRotation();
			const FRotator Yaw_rotation(0.f, control_rotation.Yaw, 0.f);

			const FVector direction_rotated = FRotationMatrix(Yaw_rotation).GetUnitAxis(axis);
			AddMovementInput(direction_rotated, value, true);
		}

		//	Set Ground State
		if (m_is_sprint && m_action_state != EActionState::EAS_Consume)
		{
			m_attribute->ConsumeStamina(0.1f);
			m_hud_overlay->RefreshStatBar(EOverlayStatType::EOST_Stamina, m_attribute->GetStaminaPercent(), false);

			m_ground_state = EGroundState::EGS_Sprint;
			m_movement_component->MaxWalkSpeed = 600.f;

			UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(GetMesh()->GetAnimInstance());
			if (nullptr == anim_instance) return;

			const FVector last_input = GetLastMovementInputVector().GetSafeNormal2D();
			const FVector forward	 = GetActorForwardVector();

			double theta = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(forward, last_input)));

			bool possible = anim_instance->IsEnableSprintTurn();
			if (FMath::Abs(theta) >= 135.f && possible)
			{
				PlayMontage(m_montage_ground, FName("Forward_Turn"));
				m_action_state = EActionState::EAS_Turning;

				m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = false;
				anim_instance->ResetEnableSprintTurn();
			}
		}
		else
		{
			if (m_action_state != EActionState::EAS_Consume)
			{
				bool 	 is_walk = m_player_controller->IsInputKeyDown(EKeys::LeftControl);
				m_ground_state   = is_walk ? EGroundState::EGS_Walk : EGroundState::EGS_Jog;
				m_movement_component->MaxWalkSpeed = is_walk ? 150.f : 300.f;
			}
			else
			{
				m_ground_state = EGroundState::EGS_Walk;
				m_movement_component->MaxWalkSpeed = 150.f;
			}

			float percent = m_attribute->GetStaminaPercent();
			if (1 == percent) return;
		}
	}
	else if (m_enable_input_movement)
	{
		StopAllMontage(0.5f);
		m_action_state		= EActionState::EAS_Unoccupied;
		m_attack_name_prev	= FName();
		m_movement_component->MaxWalkSpeed = 300.f;
	}
}

void AC0000::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AC0000::Turn(float value)
{
	if ((m_lock_on_state == ELockOnState::ELOS_LockOn || m_direction_state != EDirectionState::EDS_DirectionOff) &&
		false	== m_timeline_camera_focus.IsPlaying() &&
		nullptr != m_actor_target)
	{
		FRotator rotator_direction = UKismetMathLibrary::MakeRotFromX(m_actor_target->GetActorLocation() - GetActorLocation());
		FRotator rotator_controll  = m_player_controller->GetControlRotation();

		m_player_controller->SetControlRotation(FRotator(rotator_controll.Pitch, rotator_direction.Yaw, rotator_direction.Roll));
	}
	else if (m_direction_state == EDirectionState::EDS_DirectionOff)
	{
		AddControllerYawInput(value);
	}
}

void AC0000::LockOnTargetChange(float value)
{
	if (m_lock_on_state != ELockOnState::ELOS_LockOn || m_lock_on_list.IsEmpty()) return;

	if (0 < value)
	{
		++m_lockon_index;
		if (m_lock_on_list.Num() <= m_lockon_index) { m_lockon_index = 0; }
	}
	else if (0 > value)
	{
		--m_lockon_index;
		if (0 > m_lockon_index) { m_lockon_index = m_lock_on_list.Num() - 1; }
	}
	
	if (0 != value)
	{
		if (false == m_lock_on_list.IsValidIndex(m_lockon_index)) { m_lockon_index = 0; }

		m_actor_target->SetActiveLockOn(false);
		m_actor_target = m_lock_on_list[m_lockon_index];
		m_actor_target->SetActiveLockOn(true);

		m_timeline_camera_focus.PlayFromStart();
	}
}

void AC0000::Interaction()
{
	AItem* item = Cast<AItem>(m_overlapping_item);
	if (nullptr == item) return;

	bool is_success_add = m_inventory_component->TryAddItem(item->GetItemObject());
	if (is_success_add)
	{
		item->Destroy();
		m_overlapping_item = nullptr;
	}
}

void AC0000::Equip()
{
	if (false == IsInputPossible()) return;

	const bool is_left	= m_player_controller->IsInputKeyDown(EKeys::Left);
	const bool is_right	= m_player_controller->IsInputKeyDown(EKeys::Right);

	CHECK_INVALID(m_montage_swap)
	FString section_name = "Swap";
	if	(is_left)	{ section_name += "_L"; m_swap_hand = EWeaponEquipHand::EWEH_Left; }
	else if (is_right)	{ section_name += "_R"; m_swap_hand = EWeaponEquipHand::EWEH_Right; }
	PlayMontage(m_montage_swap, FName(section_name));

	if (is_right)
	{
		bool is_unequip = EEquipState::EES_Unequipped == m_equip_state;
		m_equip_state	= is_unequip ? EEquipState::EES_EquippedOneHandedWeapon : EEquipState::EES_Unequipped;
	}

	m_action_state	= EActionState::EAS_Equipping;
}

void AC0000::Attack()
{
	if (IsInputPossible()	== false				||
   	    m_action_state	== EActionState::EAS_Guarding		||
	    m_equip_state	== EEquipState::EES_Unequipped		||
	    m_enable_attack	== false				||
	    0 >= m_attribute->GetStaminaCurrent()			||
	    m_equiped_weapon_R	== nullptr && m_equiped_weapon_R->IsUsingWeapon()) return;
	
	if (m_is_enable_execution) 
	{
		AGameCharacter* target = Cast<AGameCharacter>(m_actor_execution_target);
		if (nullptr == target) return;

		EGameDirection direction = FindDirection(m_actor_execution_target, GetActorLocation());
		if (direction == EGameDirection::EGD_Front || direction == EGameDirection::EGD_Back) {
			m_enable_attack		= false;
			m_enable_input_movement = false;

			FString post_fix = (direction == EGameDirection::EGD_Back) ? "_Back" : "_Front";
			PlayMontage(m_montage_execution, FName("Execution" + post_fix));

			if (m_actor_target) { m_actor_target->SetActiveLockOn(false); }
			m_actor_target = target;

			m_lock_on_state = ELockOnState::ELOS_LockOff;
			m_movement_component->bOrientRotationToMovement = true;

			m_timeline_camera_focus.PlayFromStart();
			if (direction == EGameDirection::EGD_Back) { m_timeline_camera_execution_back.PlayFromStart(); }
			else 					   { m_timeline_camera_execution_front.PlayFromStart(); }

			target->TakeExecution(this, direction, m_equiped_weapon_R->GetWeaponDamage(EAttackType::EATKT_ParryAttack));

			m_action_state		= EActionState::EAS_Execution;
			m_direction_state	= EDirectionState::EDS_DirectionOn;
			m_is_not_damage_mod 	= true;
		}
	}
	else
	{
		if (m_is_sprint)
		{
			m_attack_name_prev	= m_player_controller->IsInputKeyDown(EKeys::C) ? FName("Dash_Attack_Strong") : FName("Dash_Attack");
			m_attack_type_prev	= EAttackType::EATKT_DashAttack;

			m_action_state		= EActionState::EAS_Attacking;
			m_attack_strength	= EAttackStrength::EATKS_Normal;
			m_battle_pose		= EBattlePose::EBP_Attack;
			m_enable_input_movement	= false;
			m_enable_attack		= false;

			m_attribute->ConsumeStamina(30);
			m_hud_overlay->RefreshStatBar(EOverlayStatType::EOST_Stamina, m_attribute->GetStaminaPercent(), false);

			PlayMontage(m_montage_attack, FName(m_attack_name_prev));

			m_equiped_weapon_R->SetAttackWeight(m_equip_state, EAttackStrength::EATKS_Normal, EAttackType::EATKT_Attack);
		}
		else if (m_jump_state > EJumpState::EJS_Unoccupied) { m_attack_type_prev = EAttackType::EATKT_JumpAttack; }
		else
		{
			if (m_player_controller->IsInputKeyDown(EKeys::C)) 
			{
				FString attack_name = m_attack_name_prev.ToString();

				if	(attack_name.Contains("Attack_Strong_01"))	m_attack_name_prev = FName("Attack_Strong_02");
				else if (attack_name.Contains("Attack_Strong_02"))	m_attack_name_prev = FName("Attack_Strong_01");
				else							m_attack_name_prev = FName("Attack_Strong_01");

				m_attack_strength = EAttackStrength::EATKS_Strong;
				m_equiped_weapon_R->SetAttackWeight(m_equip_state, EAttackStrength::EATKS_Strong, EAttackType::EATKT_Attack);
			}
			else {
				if	(FName("Attack_01") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_02");
				else if (FName("Attack_02") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_03");
				else if (FName("Attack_03") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_04");
				else if (FName("Attack_04") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_05");
				else							m_attack_name_prev = FName("Attack_01");

				m_enable_attack_short	= false;
				m_attack_strength		= EAttackStrength::EATKS_Normal;
				m_equiped_weapon_R->SetAttackWeight(m_equip_state, EAttackStrength::EATKS_Normal, EAttackType::EATKT_Attack);

				m_attribute->ConsumeStamina(20);
				m_hud_overlay->RefreshStatBar(EOverlayStatType::EOST_Stamina, m_attribute->GetStaminaPercent(), false);
			}

			m_attack_type_prev		= EAttackType::EATKT_Attack;
			m_action_state			= EActionState::EAS_Attacking;
			m_battle_pose			= EBattlePose::EBP_Attack;
			m_enable_input_movement		= false;
			m_enable_attack			= false;

			m_enable_input_attack_rotate	= true;
			m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;

			PlayMontage(m_montage_attack, FName(m_attack_name_prev));
		}
	}
}

void AC0000::ShortAttack()
{
	if (m_enable_attack_short)
	{
		if	(FName("Attack_Strong_01") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_Strong_01_Short");
		else if (FName("Attack_Strong_02") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_Strong_02_Short");

		m_equiped_weapon_R->SetAttackWeight(m_equip_state, EAttackStrength::EATKS_Normal, EAttackType::EATKT_Attack);
		PlayMontage(m_montage_attack, FName(m_attack_name_prev));

		m_enable_attack_short = false;
	}
}

void AC0000::LockOn()
{
	if (m_lock_on_state == ELockOnState::ELOS_LockOn) 
	{
		m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CHECK_INVALID(m_actor_target)
		m_actor_target->SetActiveLockOn(false);
		m_actor_target = nullptr;

		m_lock_on_state = ELockOnState::ELOS_LockOff;
		m_movement_component->bOrientRotationToMovement = true;
	}
	else 
	{
		m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		if (nullptr != m_actor_target) 
		{
			m_lock_on_state = ELockOnState::ELOS_LockOn;
			m_actor_target->SetActiveLockOn(true);
			m_movement_component->bOrientRotationToMovement = false;

			m_timeline_camera_focus.PlayFromStart();
		}
		else {
			TDelegate lockon_delegate = FTimerDelegate::CreateLambda([this]()->void { 
				if (nullptr == m_actor_target) {
					GetWorldTimerManager().ClearTimer(m_timer_lockon); 
					m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}});
			GetWorldTimerManager().SetTimer(m_timer_lockon, lockon_delegate, 1, false);
		}
	}
}

void AC0000::Dodge()
{
	if (nullptr == m_movement_component			||
    	    false   == IsInputPossible()			||
	    m_action_state == EActionState::EAS_Attacking	||
	    (0 >= m_attribute->GetStaminaCurrent())) return;

	m_action_state = EActionState::EAS_Dodgeing;
	m_ground_state = EGroundState::EGS_Unoccupied;

	float	   scale_x	= 0;
	float	   scale_y	= 0;
	const bool input_axis_x = IsInputKey(FName("MoveForward"), scale_x, true);
	const bool input_axis_y = IsInputKey(FName("MoveSide"), scale_y, true);
	FString section_name 	= (input_axis_x || input_axis_y) ? FString("Dodge_Front") : FString("Dodge_Default");

	if (ELockOnState::ELOS_LockOn == m_lock_on_state)
	{
		if (input_axis_x && scale_x < 0) { section_name = FString("Dodge_Back"); }
		else if (input_axis_y)
		{
			if	(scale_y > 0)    { section_name = FString("Dodge_Right"); }
			else if (scale_y < 0)    { section_name = FString("Dodge_Left"); }
		}
	}

	m_attribute->ConsumeStamina(10.f);
	m_hud_overlay->RefreshStatBar(EOverlayStatType::EOST_Stamina, m_attribute->GetStaminaPercent(), false);

	PlayMontage(m_montage_dodge, FName(section_name));
}

void AC0000::Guard()
{
	CHECK_INVALID(m_player_controller)
	AShield_Actor* shield = Cast<AShield_Actor>(m_equiped_weapon_L);

	if (IsInputPossible()	== false			||
		m_action_state	== EActionState::EAS_Attacking  ||
		m_jump_state	!= EJumpState::EJS_Unoccupied	||
		shield == nullptr) return;

	shield->SetShieldCollision(ECollisionEnabled::QueryOnly);

	bool is_key_down = m_player_controller->IsInputKeyDown(EKeys::RightMouseButton);
	if (is_key_down) { m_action_state = EActionState::EAS_Guarding; }

	FString section_name = is_key_down ? FString("GuardStart") : FString("GuardEnd");
	if (false == IsInputAxisActionKey())
	{
		section_name += FString("_Idle");
		PlayMontage(m_montage_guard, FName(section_name), 1.0, true, 0.25);
	}
}

void AC0000::Inventory()
{
	CHECK_INVALID(m_inventory_component)
	m_inventory_component->ToggleInventory();
}

void AC0000::Parry()
{
	if (EActionState::EAS_Parry == m_action_state) { return; }

	AShield_Actor* shield = Cast<AShield_Actor>(m_equiped_weapon_L);
	CHECK_INVALID(shield)

	m_action_state = EActionState::EAS_Parry;
	shield->SetShieldCollision(ECollisionEnabled::NoCollision);

	PlayMontage(m_montage_guard, FName("Guard_Parry"));
}

void AC0000::Consume()
{
	if (m_action_state != EActionState::EAS_Unoccupied) { return; }

	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID(anim_instance)

	anim_instance->SetBlendPerBone(true);
	m_action_state = EActionState::EAS_Consume;

	FName section_name = (m_total_potion > 0) ? FName("Consume_Start") : FName("Consume_Fail");
	PlayMontage(m_montage_potion_consume, section_name);

	double standard	= m_total_potion * 0.5;
	m_total_potion	= (m_total_potion > 0) ? (m_total_potion - 1) : 0;
	EPotionSizeType size = (m_total_potion > (standard)) ? EPotionSizeType::EPST_Half : EPotionSizeType::EPST_Less;
	if (m_total_potion <= 0) { size = EPotionSizeType::EPST_Empty; }
	
	m_hud_overlay->SetQuickSlotItem(EQuickSlotType::EQST_Bottom, nullptr, size);
}

void AC0000::StartJump()
{
	if (EActionState::EAS_Attacking == m_action_state ||
	    EActionState::EAS_Equipping == m_action_state ||
	    EActionState::EAS_Dodgeing	== m_action_state ||
	    EActionState::EAS_Turning	== m_action_state ||
	    EJumpState::EJS_JumpStart	== m_jump_state) return;

	if (nullptr == m_movement_component || !CanJump()) return;

	m_jump_state		= EJumpState::EJS_JumpStart;
	float speed_cur 	= m_movement_component->Velocity.Size2D();
	FString section_name 	= (150.f <= speed_cur) ? FString("Jump_Front") : FString("Jump_Standing");

	if (ELockOnState::ELOS_LockOn == m_lock_on_state)
	{
		float scale = 0;
		if (IsInputKey(FName("MoveForward"), scale, true))
		{
			if 	(scale < 0)	{ section_name = FString("Jump_Back"); }
		}
		else if (IsInputKey(FName("MoveSide"), scale, true))
		{
			if	(scale > 0)	{ section_name = FString("Jump_Right"); }
			else if (scale < 0)	{ section_name = FString("Jump_Left"); }
		}
	}

	if (300.f <= speed_cur) section_name += FString("_Medium");
	PlayMontage(m_montage_jump, FName(section_name));
}

void AC0000::OnDetectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGameCharacter* character = Cast<AGameCharacter>(OtherActor);
	if (character == nullptr || this == character) return;

	if (IsInViewport(character->GetActorLocation()))
	{
		m_lock_on_list.AddUnique(character);
		if (0 >= m_lock_on_list.Num()) return;

		FVector user_location = GetActorLocation();
		Algo::Sort(m_lock_on_list, [user_location](AGameCharacter* left, AGameCharacter* right)
			{
				return (left->GetActorLocation() - user_location).Size() < (right->GetActorLocation() - user_location).Size();
			});

		if (nullptr == m_actor_target)
		{
			m_lock_on_state = ELockOnState::ELOS_LockOn;
			m_actor_target  = m_lock_on_list[0];
			m_actor_target->SetActiveLockOn(true);
			m_movement_component->bOrientRotationToMovement = false;

			m_timeline_camera_focus.PlayFromStart();
		}
	}
}

void AC0000::OnDetectEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGameCharacter* character = Cast<AGameCharacter>(OtherActor);
	if (character == nullptr || this == character) { return; }

	for (AGameCharacter* const target : m_lock_on_list)
	{
		if (character != target) continue;
		
		if (m_actor_target == target) 
		{
			m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			m_lock_on_state = ELockOnState::ELOS_LockOff;
			m_movement_component->bOrientRotationToMovement = true;

			if (nullptr == m_actor_target) return;
			m_actor_target->SetActiveLockOn(false);
			m_actor_target = nullptr;
		}

		m_lock_on_list.Remove(character);
		return;
	}
}

bool AC0000::IsInViewport(const FVector& target_location)
{
	const APlayerController* const PlayerController = Cast<const APlayerController>(GetController());
	if (nullptr == PlayerController) return false;

	FVector2D location_viewport;
	PlayerController->ProjectWorldLocationToScreen(target_location, location_viewport);

	int32 viewport_x, viewport_y;
	PlayerController->GetViewportSize(viewport_x, viewport_y);

	return ((location_viewport.X > 0 && location_viewport.Y > 0) && (location_viewport.X < viewport_x && location_viewport.Y < viewport_y));
}

void AC0000::EquipArmorFromInventory(UItemObject* const item_object)
{
	if (nullptr == item_object || nullptr == m_inventory_component) return;

	EEquipmentType equip_type = item_object->GetItemEquipType();
	if (equip_type == EEquipmentType::EET_None || 
	    equip_type == EEquipmentType::EET_Max  ||
	    equip_type == EEquipmentType::EET_Weapon) return;

	UWorld* world = GetWorld();
	CHECK_INVALID(world)

	int32 index = int32(equip_type) - 1;
	if (false == m_equiped_item.IsValidIndex(index)) return;

	USkeletalMeshComponent* new_mesh_component = NewObject<USkeletalMeshComponent>(this, UEnum::GetValueAsName(equip_type));
	new_mesh_component->SetupAttachment(GetMesh());

	FItemData* item_data = item_object->GetItmeData();
	USkeletalMesh* new_skeletal_mesh = LoadObject<USkeletalMesh>(nullptr, *item_data->MeshPath);
	CHECK_INVALID(new_skeletal_mesh)

	new_mesh_component->SetSkeletalMesh(new_skeletal_mesh);
	new_mesh_component->SetLeaderPoseComponent(GetMesh());
	new_mesh_component->RegisterComponent();

	m_equiped_item[index].Key   = new_mesh_component;
	m_equiped_item[index].Value = item_object;

	MeshActivate(item_object, false);
}

void AC0000::EquipWeaponFromInventory(UItemObject* const item_object, const bool& is_right_hand)
{
	if (nullptr == item_object || nullptr == m_inventory_component) { return; }

	EEquipmentType equip_type = item_object->GetItemEquipType();
	if (equip_type == EEquipmentType::EET_None ||
	    equip_type == EEquipmentType::EET_Max ||
	    equip_type != EEquipmentType::EET_Weapon) return;

	UWorld* world = GetWorld();
	CHECK_INVALID(world)

	AWeapon_Actor* weapon = world->SpawnActor<AWeapon_Actor>(item_object->GetItemClass());
	CHECK_INVALID(weapon)

	FString	socket_name     = is_right_hand ? "R" : "L";
	EWeaponType weapon_type = weapon->GetWeaponType();

	if (EWeaponType::EWT_Shield == weapon_type) { socket_name += FString("_ShieldSocket"); }
	else { socket_name += FString("_WeaponSocket"); }

	weapon->Equip(GetMesh(), FName(socket_name), this, this);

	AWeapon_Actor* equiped_weapon = nullptr;
	if (is_right_hand) { m_equiped_weapon_R = weapon; equiped_weapon = m_equiped_weapon_R; }
	else 		   { m_equiped_weapon_L = weapon; equiped_weapon = m_equiped_weapon_L; }

	m_hud_overlay->SetQuickSlotItem(is_right_hand ? EQuickSlotType::EQST_Right : EQuickSlotType::EQST_Left, equiped_weapon->GetItemObject());
	equiped_weapon->SetWeaponUsing(true);
}

void AC0000::UnequipArmor(const EEquipmentType& equip_type)
{
	CHECK_INVALID(m_inventory_component);
	if (equip_type == EEquipmentType::EET_None ||
	    equip_type == EEquipmentType::EET_Max  ||
	    equip_type == EEquipmentType::EET_Weapon) return;

	UWorld* world = GetWorld();
	CHECK_INVALID(world)

	int32 index = int32(equip_type) - 1;
	if (false == m_equiped_item.IsValidIndex(index)) return;

	CHECK_INVALID(m_equiped_item[index].Key)
	m_equiped_item[index].Key->SetSkeletalMesh(nullptr);
	m_equiped_item[index].Key->UnregisterComponent();
	m_equiped_item[index].Key = nullptr;

	CHECK_INVALID(m_equiped_item[index].Value)
	MeshActivate(m_equiped_item[index].Value, true);
	m_equiped_item[index].Value = nullptr;
}

void AC0000::UnequipWeapon(const EEquipmentType& equip_type, const bool& is_right_hand)
{
	CHECK_INVALID(m_inventory_component);
	if (equip_type != EEquipmentType::EET_Weapon ) return;

	UWorld* world = GetWorld();
	CHECK_INVALID(world)

	if (is_right_hand && nullptr != m_equiped_weapon_R) 
	{
		m_equiped_weapon_R->Destroy();
		m_equiped_weapon_R = nullptr;
	}
	else if (nullptr != m_equiped_weapon_L) 
	{
		m_equiped_weapon_L->Destroy();
		m_equiped_weapon_L = nullptr;
	}
}

void AC0000::MeshActivate(UItemObject* const item_object, const bool& is_show)
{
	FItemData* item_data = item_object->GetItmeData();
	CHECK_INVALID(item_data)

	USkeletalMeshComponent* mesh_component = GetMesh();
	CHECK_INVALID(mesh_component)

	USkeletalMesh* skeletal_mesh = mesh_component->GetSkeletalMeshAsset();
	CHECK_INVALID(skeletal_mesh)

	FSkeletalMeshModel* skeletal_mesh_model = skeletal_mesh->GetImportedModel();
	CHECK_INVALID(skeletal_mesh_model)

	TArray<FSkelMeshSection>& sections = skeletal_mesh_model->LODModels[0].Sections;
	int32 section_count = sections.Num();

	USkinnedMeshComponent* SkinnedMeshComp = FindComponentByClass<USkinnedMeshComponent>();
	CHECK_INVALID(SkinnedMeshComp)

	int32 hide_index = 0;
	const TArray<int32>& hide_materials = item_data->HideMaterials;
	for (const int32& material_index : hide_materials)
	{
		hide_index = FMath::Clamp(material_index, 0, section_count - 1);
		SkinnedMeshComp->ShowMaterialSection(hide_index, 0, is_show, 0);
	}
}

void AC0000::Swap_Weapon(const EWeaponEquipHand& hand_type)
{
	if (hand_type == EWeaponEquipHand::EWEH_Right)
	{
		CHECK_INVALID(m_equiped_weapon_R)
		bool is_equip = m_equiped_weapon_R->IsUsingWeapon();
		m_equiped_weapon_R->AttachMashToSocket(GetMesh(), is_equip ? FName("L_HipSocket") : FName("R_WeaponSocket"));
		m_equiped_weapon_R->SetWeaponUsing(!is_equip);
		
		if (is_equip) 	{ m_hud_overlay->SetQuickSlotItem(EQuickSlotType::EQST_Right); }
		else 		{ m_hud_overlay->SetQuickSlotItem(EQuickSlotType::EQST_Right, m_equiped_weapon_R->GetItemObject()); }
	}
	else
	{
		CHECK_INVALID(m_equiped_weapon_L)
		bool is_equip = m_equiped_weapon_L->IsUsingWeapon();
		m_equiped_weapon_L->AttachMashToSocket(GetMesh(), is_equip ? FName("Spine1_MantleSocket") : FName("L_ShieldSocket"));
		m_equiped_weapon_L->SetWeaponUsing(!is_equip);
		
		if (is_equip) 	{ m_hud_overlay->SetQuickSlotItem(EQuickSlotType::EQST_Left); }
		else 		{ m_hud_overlay->SetQuickSlotItem(EQuickSlotType::EQST_Left, m_equiped_weapon_L->GetItemObject()); }
	}
}

void AC0000::GetJumpDirectionLength(const FVector& apex_location)
{
	const FVector direction	= apex_location - m_location_jump_start;
	const double  length	= direction.Size2D();

	AddMovementInput(direction, length, true);
}

bool AC0000::IsInputKey(const FName& Input_name, float& scale, const bool& is_axis)
{
	if (nullptr == m_player_controller) return false;

	if (is_axis)
	{
		TArray<FInputAxisKeyMapping> axis_keys = m_player_controller->PlayerInput->GetKeysForAxis(Input_name);
		for (const FInputAxisKeyMapping& key : axis_keys)
		{
			if (m_player_controller->IsInputKeyDown(key.Key))
			{
				scale = key.Scale;
				return true;
			}
		}
	}
	else
	{
		TArray<FInputActionKeyMapping> action_keys = m_player_controller->PlayerInput->GetKeysForAction(Input_name);
		for (const FInputActionKeyMapping& key : action_keys)
		{
			if (m_player_controller->IsInputKeyDown(key.Key)) return true;
		}
	}

	return false;
}

bool AC0000::IsInputAxisActionKey()
{
	if (nullptr == m_player_controller) return false;
	
	float scale = 0;
	if (IsInputKey(FName("MoveForward"), scale, true) ||
	    IsInputKey(FName("MoveSide"), scale, true)	  ||
	    IsInputKey(FName("Attack"), scale, false)	  ||
	    IsInputKey(FName("Jump"), scale, true)	  ||
	    IsInputKey(FName("Dodge"), scale, true)) return true;

	return false;
}

bool AC0000::IsInputKeyByFKey(const FKey& key)
{
	if (nullptr == m_player_controller) return false;
	
	return m_player_controller->IsInputKeyDown(key);
}

void AC0000::OnEndTurn()
{
	m_action_state = EActionState::EAS_Unoccupied;

	if (m_is_sprint)
	{
		UAnimInstance* anim_instance = GetMesh()->GetAnimInstance();
		CHECK_INVALID(anim_instance);

		anim_instance->StopAllMontages(0.5f);
	}
}

void AC0000::OnEndAttack()
{
	m_action_state		= EActionState::EAS_Unoccupied;
	
	m_attack_name_prev	= FName();
	m_attack_type_prev	= EAttackType::EATKT_None;
	m_attack_strength	= EAttackStrength::EATKS_None;
	m_battle_pose		= EBattlePose::EBP_Unoccupied;
	m_enable_attack		= true;
}

void AC0000::OnEndDirection()
{
	m_direction_state   = EDirectionState::EDS_DirectionOff;
	m_is_not_damage_mod = false;
}

void AC0000::OnDodgeEnd()
{
	StopAllMontage(0.5f);
}

void AC0000::OnGuardEnd()
{
	StopAllMontage(0.5f);
}

void AC0000::OnStartKnockback()
{
	m_enable_input_movement		= false;
	m_enable_input_attack_rotate	= false;

	m_action_state = EActionState::EAS_Knockbacking;
}

void AC0000::OnEndKnockback()
{
	m_enable_input_movement = true;
	m_action_state = EActionState::EAS_Unoccupied;
}

void AC0000::OnCameraFocusRotate(float curve_value)
{
	if (nullptr == m_actor_target)
	{
		m_timeline_camera_focus.Stop();
		return;
	}

	FRotator rotator_current	= m_player_controller->GetControlRotation();
	FRotator rotator_direction	= UKismetMathLibrary::MakeRotFromX(m_actor_target->GetActorLocation() - GetActorLocation());
	FRotator rotator_goal		= FRotator(rotator_current.Pitch, rotator_direction.Yaw, rotator_direction.Roll);
	FRotator rotation_calc		= UKismetMathLibrary::RInterpTo(m_player_controller->GetControlRotation(), rotator_goal, curve_value, 10);

	m_player_controller->SetControlRotation(FRotator(rotator_current.Pitch, rotation_calc.Yaw, rotation_calc.Roll));
	
	if (rotation_calc == rotator_goal) { m_timeline_camera_focus.Stop(); }
}

void AC0000::OnCameraExecutionFront(float curve_value)
{
	if (nullptr == m_actor_target)
	{
		m_timeline_camera_execution_front.Stop();
		return;
	}
	m_spring_arm->TargetArmLength   = m_spring_arm_length - (curve_value * 150);

	FRotator rotator_current	= m_player_controller->GetControlRotation();
	FRotator rotator_direction	= UKismetMathLibrary::MakeRotFromX(m_actor_target->GetActorLocation() - GetActorLocation());
	FRotator rotator_goal		= FRotator(340.f, rotator_direction.Yaw, rotator_direction.Roll);
	FRotator rotation_calc		= UKismetMathLibrary::RInterpTo(m_player_controller->GetControlRotation(), rotator_goal, curve_value, 10);

	m_player_controller->SetControlRotation(FRotator(rotation_calc.Pitch, rotation_calc.Yaw, rotation_calc.Roll));
}

void AC0000::OnCameraExecutionBack(float curve_value)
{
	if (nullptr == m_actor_target)
	{
		m_timeline_camera_execution_back.Stop();
		return;
	}

	m_spring_arm->TargetArmLength = m_spring_arm_length - (curve_value * 150);

	FRotator rotator_current	= m_player_controller->GetControlRotation();
	FRotator rotator_direction	= UKismetMathLibrary::MakeRotFromX(m_actor_target->GetActorLocation() - GetActorLocation());
	FRotator rotator_goal		= FRotator(340.f, rotator_direction.Yaw, rotator_direction.Roll);
	FRotator rotation_calc		= UKismetMathLibrary::RInterpTo(m_player_controller->GetControlRotation(), rotator_goal, curve_value, 10);

	m_player_controller->SetControlRotation(FRotator(rotation_calc.Pitch, rotation_calc.Yaw, rotation_calc.Roll));
}

void AC0000::SetWeaponCollision(ECollisionEnabled::Type type)
{
	CHECK_INVALID(m_equiped_weapon_R)

	AMeleeAttack_Actor* weapon = Cast<AMeleeAttack_Actor>(m_equiped_weapon_R);
	CHECK_INVALID(weapon)

	auto collision_box = weapon->GetWeaponBox();
	if (nullptr != collision_box) { collision_box->SetCollisionEnabled(type); }
	m_equiped_weapon_R->m_ignore_actors.Empty();
}

void AC0000::SetGroundUnoccupied()
{
	m_ground_state = EGroundState::EGS_Unoccupied;

	float percent = m_attribute->GetStaminaPercent();
	if (1 == percent) return;
}

void AC0000::SetGold(const int32& gold)
{
	m_total_gold += gold;
	m_inventory_component->RefreshGoldAmount(m_total_gold);
}

