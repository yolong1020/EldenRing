// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Players/C0000.h"
#include "Characters/Players/C0000_AnimInstance.h"
#include "Characters/ObjectSpawnPoint.h"
#include "Characters/Component/AttributeComponent.h"
#include "GameBase/ERGameInstance.h"
#include "GameBase/ERGameMode.h"
#include "System/StorageManager.h"
#include "System/EquipmentManager.h"
#include "System/NPCInteractionManager.h"
#include "System/TradeManager.h"
#include "System/DataTableManager.h"
#include "System/ObserverManager.h"
#include "System/UIManager.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Common/CommonFunctions.h"
#include "HUD/Status/GameHUDComponent.h"
#include "UI/Notify/YouDied.h"
#include "UI/UIDelegates.h"
#include "UI/PopUp/PopUpTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Items/ItemObject.h"
#include "Items/Weapons/Weapon_Interface.h"
#include "Items/Weapons/Weapon_Actor.h"
#include "Items/Equipment/Equipment.h"
#include "TimerManager.h"
#include "Camera/ExecutionCameraShake.h"
#include "Particles/ParticleSystemComponent.h"
#include "Levels/BaseLevelScript.h"
#include "Macro/BasicMacro.h"
#include "Macro/DebugMacros.h"

AC0000::AC0000()
{
 	PrimaryActorTick.bCanEverTick	= true;

	bUseControllerRotationYaw	= false;
	bUseControllerRotationPitch	= false;
	bUseControllerRotationRoll	= false;

	//	Movement
	m_movement_component = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	CHECK_INVALID_PTR(m_movement_component)
	
	m_movement_component->bOrientRotationToMovement = true;
	m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;
	m_movement_component->RotationRate = FRotator(0.f, 400.f, 0.f);

	//	Spring Arm
	m_spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_spring_arm->SetupAttachment(RootComponent);

	//	Camera
	m_view_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	m_view_camera->SetupAttachment(m_spring_arm);

	//	Sphere Detect
	m_sphere_detect = CreateDefaultSubobject<USphereComponent>(TEXT("SphereDetect"));
	m_sphere_detect->SetupAttachment(RootComponent);

	//	HUD
	m_hud_component = CreateDefaultSubobject<UGameHUDComponent>(TEXT("HUDComponent"));
	m_hud_component->SetupAttachment(RootComponent);
}

void AC0000::SetDefaultItems()
{
	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)

	auto EquipItem = [&](const FName& Name, const bool IsRight)->void {
		if (Name.IsNone()) return;

		UDataTableManager* datatable_mgr = instance->GetSubsystem<UDataTableManager>();
		CHECK_INVALID_PTR(datatable_mgr)

		UDataTable* item_table = datatable_mgr->GetDataTable(EDataTableType::EDTT_Item);
		CHECK_INVALID_PTR(item_table)

		FItemData* item_data = item_table->FindRow<FItemData>(Name, Name.ToString());
		CHECK_INVALID_PTR(item_data)

		UClass* item_class = LoadClass<AItem>(nullptr, *item_data->BluePrintPath);
		CHECK_INVALID_PTR(item_class)

		TObjectPtr<UItemObject> item_object = NewObject<UItemObject>();
		item_object->SetItemProperties(Name, *item_data, item_class, false);

		EEquipmentType type = item_object->GetItemEquipType();
		m_equipment_mgr->TryEquipItem(item_object, item_object->GetItemEquipType(), IsRight);
	};

	EquipItem(m_name_weapon_R, true);
	EquipItem(m_name_weapon_L, false);
	EquipItem(m_starter_head, true);
	EquipItem(m_starter_body, true);
	EquipItem(m_starter_arms, true);
	EquipItem(m_starter_legs, true);
}

void AC0000::AdjustItemOption(const bool& IsEquip, const TObjectPtr<UItemObject> ItemObject)
{
	CHECK_INVALID_PTR(m_attribute)
	if (IsEquip) { m_attribute->AddOffset(ItemObject); }
	else	     { m_attribute->RemoveOffset(ItemObject); }

	CHECK_INVALID_PTR(m_hud_component)
	m_hud_component->UpdateStatusBar();
}

void AC0000::SetWeapon(const bool& IsRight, AWeapon_Actor* const Weapon)
{
	if (IsRight) 
	{ 
		m_equiped_weapon_R = Weapon;
		m_equip_state = EEquipState::EES_EquippedOneHandedWeapon; 
	}
	else
	{
		m_equiped_weapon_L = Weapon;
	}
}

void AC0000::BeginPlay()
{
	UERGameInstance* instance = Cast<UERGameInstance>(UGameplayStatics::GetGameInstance(this));
	CHECK_INVALID_PTR(instance)

	ABaseLevelScript* level = Cast<ABaseLevelScript>(GetWorld()->GetLevelScriptActor());
	if (!instance || !level) return;

	if (!level->IsPlayLevel() || !instance->IsLevelLoadComplete())
	{
		APlayerController* controller = Cast<APlayerController>(Controller);
		CHECK_INVALID_PTR(controller)

		controller->bShowMouseCursor = true;
		return;
	}

	Super::BeginPlay();
	UCharacterMovementComponent* movement = GetCharacterMovement();
	movement->SetMovementMode(EMovementMode::MOVE_Walking);
	Tags.Add(FName("Player"));

	m_overlapping_item	= nullptr;
	m_enable_attack		= true;
	m_enable_attack_short	= false;
	m_enable_sprint_turn	= false;
	m_is_not_damage_mod	= false;
	m_cur_potion		= 3;

	m_spring_arm_length = m_spring_arm->TargetArmLength;

	CHECK_INVALID_PTR(m_sphere_detect)
	m_sphere_detect->OnComponentBeginOverlap.AddDynamic(this, &AC0000::OnDetectOverlap);
	m_sphere_detect->OnComponentEndOverlap.AddDynamic(this, &AC0000::OnDetectEnd);
	m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CHECK_INVALID_PTR(m_hud_component)
	m_hud_component->InitHUD(m_attribute);
	m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Bottom, nullptr, EPotionSizeType::EPST_Full);

	m_storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_storage_mgr->InitStorageWidget(this);
	UTradeManager* trade_mgr = instance->GetSubsystem<UTradeManager>();
	CHECK_INVALID_PTR(trade_mgr)
	trade_mgr->InitTradeWidget(m_storage_mgr->GetSlots());
	m_equipment_mgr = instance->GetSubsystem<UEquipmentManager>();
	CHECK_INVALID_SMART_PTR(m_equipment_mgr)
	m_interact_mgr = instance->GetSubsystem<UInteractionManager>();
	CHECK_INVALID_SMART_PTR(m_interact_mgr)
	m_ui_mgr = instance->GetSubsystem<UUIManager>();
	CHECK_INVALID_SMART_PTR(m_ui_mgr)

	instance->IsNewGame() ? SetDefaultItems() : instance->LoadPlayer(this);

	//	Camera Timeline
	FOnTimelineFloat tl_focus_callback;
	FOnTimelineFloat tl_execution_front;
	FOnTimelineFloat tl_execution_back;
	FOnTimelineFloat tl_rest;
	tl_focus_callback.BindUFunction(this, FName("OnCameraFocusRotate"));
	tl_execution_front.BindUFunction(this, FName("OnCameraExecutionFront"));
	tl_execution_back.BindUFunction(this, FName("OnCameraExecutionBack"));
	tl_rest.BindUFunction(this, FName("OnCameraRest"));
	m_tl_cam_focus.AddInterpFloat(m_curve_camera_focus, tl_focus_callback);
	m_tl_cam_execution_front.AddInterpFloat(m_curve_camera_execution_front, tl_execution_front);
	m_tl_cam_execution_back.AddInterpFloat(m_curve_camera_execution_back, tl_execution_back);
	m_tl_cam_rest.AddInterpFloat(m_curve_camera_rest_end, tl_rest);

	m_anim_instance = Cast<UC0000_AnimInstance>(GetMesh()->GetAnimInstance());
	CHECK_INVALID_PTR(m_anim_instance)

	m_equip_state = EEquipState::EES_EquippedOneHandedWeapon;

	const bool load_complete = instance->IsLevelLoadComplete();
	load_complete ? GameStartAction() : instance->AddEventLevelLoadComplete(this, &AC0000::GameStartAction);
}

void AC0000::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	m_tl_cam_focus.TickTimeline(DeltaTime);
	m_tl_cam_execution_front.TickTimeline(DeltaTime);
	m_tl_cam_execution_back.TickTimeline(DeltaTime);
	m_tl_cam_rest.TickTimeline(DeltaTime);
}

void AC0000::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (Controller) { 
		m_player_controller = Cast<APlayerController>(Controller);
		m_player_controller->bShowMouseCursor = false;
		DisableInput(m_player_controller.Get());
	}
}

void AC0000::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	m_jump_state 		= EJumpState::EJS_Unoccupied;
	m_enable_attack 	= true;
	m_enable_attack_short 	= true;

	if (m_action_state == EActionState::EAS_HitReact) return;

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

	CHECK_INVALID_PTR(m_anim_instance)
	m_movement_component->SetMovementMode(EMovementMode::MOVE_Walking);
	m_anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

	PlayMontageSection(montage, FName(section_name));
}

void AC0000::HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight)
{
	CHECK_INVALID_PTR(m_anim_instance)

	bool	is_need_blend		= true;
	FString	hit_section_postfix	= "";
	int32	selected_index		= 0;

	UAnimMontage*	montage_with_direction	= nullptr;
	UAnimMontage*	montage_hit 		= nullptr;

	switch (attack_weight)
	{
	case EAttackWeight::EAW_Small:
	{
		hit_section_postfix	= FString("Small");
	}break;
	case EAttackWeight::EAW_Medium:
	{
		hit_section_postfix	= FString("Medium");
		montage_hit		= m_montage_hit_medium;
	} break;
	case EAttackWeight::EAW_Heavy:
	{
		hit_section_postfix	= FString("Heavy");
		montage_with_direction	= nullptr;
	} break;
	case EAttackWeight::EAW_ExtraHeavy:
	{
		hit_section_postfix	= FString("ExtraHeavy");
		montage_with_direction	= nullptr;
	} break;
	case EAttackWeight::EAW_UltraHeavy:
	{
		hit_section_postfix	= FString("UltraHeavy");
		montage_with_direction	= nullptr;
		montage_hit		= m_montage_hit_ultra;
	} break;
	}
	CHECK_INVALID_PTR(montage_hit)

	switch (hit_direction)
	{
	case EGameDirection::EGD_Front: hit_section_postfix += FString("_Front"); break;
	case EGameDirection::EGD_Back:	hit_section_postfix += FString("_Back");  break;
	case EGameDirection::EGD_Left:	hit_section_postfix += FString("_Right"); break;
	case EGameDirection::EGD_Right: hit_section_postfix += FString("_Left");  break;
	default: return;
	}

	if (attack_weight != EAttackWeight::EAW_UltraHeavy)
	{
		selected_index	    =  FMath::RandRange(1, montage_hit->GetNumSections() / 4);
		hit_section_postfix += (FString("_0") + FString::FromInt(selected_index));
	}

	PlayMontageSection(montage_hit, FName(hit_section_postfix));
	m_anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(m_anim_instance);
	CHECK_INVALID_PTR(m_anim_instance)

	bool is_already_clear = false;
	for (FAnimMontageInstance* instance : m_anim_instance->MontageInstances)
	{
		if (false	== instance->bPlaying ||
			nullptr == instance->Montage ||
			false	== instance->Montage->GetFName().ToString().Contains("AM_HitReact")) continue;

		int32 index = instance->Montage->GetSectionIndex(FName(hit_section_postfix));
		if (0 > index) continue;

		FCompositeSection section	= instance->Montage->GetAnimCompositeSection(index);
		int32		  segment_index = section.GetSegmentIndex();

		if (false == instance->Montage->SlotAnimTracks.IsValidIndex(0)) continue;

		FSlotAnimationTrack& default_slot = instance->Montage->SlotAnimTracks[0];
		if (false == default_slot.AnimTrack.AnimSegments.IsValidIndex(segment_index)) continue;

		FAnimSegment 	segment = default_slot.AnimTrack.AnimSegments[segment_index];
		float		length 	= segment.GetLength();

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

void AC0000::FadeOutCharacter(const float Duration)
{
	Super::FadeOutCharacter(Duration);

	InputComponent->ClearActionBindings();
	m_equipment_materials = m_equipment_mgr->GetAllArmorMaterials();

	UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance());
	CHECK_INVALID_PTR(instance)
	UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
	CHECK_INVALID_PTR(observer_mgr)

 	instance->ResetSaveData();
	observer_mgr->TriggerEvent(EObserverEventType::EOET_SaveData);
}

const float AC0000::UpdateFadeOut(const float& Value)
{
	float opacity = Super::UpdateFadeOut(Value);
	for (const auto& equipment : m_equipment_materials)
	{
		for (const auto& iter : equipment)
		{
			iter->SetScalarParameterValue(FName("Opacity"), opacity);
		}
	}

	return opacity;
}

void AC0000::FinishFadeOut(UNiagaraComponent* PSystem)
{
	Super::FinishFadeOut(PSystem);

	m_equipment_mgr->UnequipAllSlot();
	m_storage_mgr->ClearStorage();
	
	UClass* widget_class = LoadClass<UYouDied>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/Notify/WBP_YouDied.WBP_YouDied_C'"));
	CHECK_INVALID_PTR(widget_class)
	UYouDied* notify_widget = CreateWidget<UYouDied>(GetWorld(), widget_class);
	CHECK_INVALID_PTR(notify_widget)

	FAfterPlayerDeathFunc Callback;
	Callback.BindUFunction(this, FName("UninitAndDestroy"));
	notify_widget->StartDirecting(Callback, 2);
}

void AC0000::OnReactEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("[Player] OnReactEnd"))
	
	CHECK_INVALID_PTR(m_movement_component)
	m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;

	CHECK_INVALID_PTR(m_anim_instance)
	m_anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

	m_enable_attack = true;
	m_action_state	= EActionState::EAS_Unoccupied;
}

bool AC0000::IsInputPossible()
{
	return	m_action_state != EActionState::EAS_Equipping	 &&
		m_action_state != EActionState::EAS_Execution	 &&
		m_action_state != EActionState::EAS_Dodgeing	 &&
		m_action_state != EActionState::EAS_Turning	 &&
		m_action_state != EActionState::EAS_HitReact	 &&
		m_action_state != EActionState::EAS_BlockReact	 &&
		m_action_state != EActionState::EAS_GuardReact	 &&
		m_action_state != EActionState::EAS_Knockbacking &&
		m_action_state != EActionState::EAS_Parry	 &&
		m_action_state != EActionState::EAS_Sit;
}

void AC0000::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AC0000::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveSide"), this, &AC0000::MoveSide);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AC0000::LookUp);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AC0000::Turn);
	PlayerInputComponent->BindAxis(FName("LockOnTargetChange"), this, &AC0000::LockOnTargetChange);
	
	PlayerInputComponent->BindAction(FName("Exit"), IE_Pressed, this, &AC0000::ExitAction);
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

void AC0000::GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight)
{
	CHECK_INVALID_PTR(m_attribute)

	if (m_jump_state > EJumpState::EJS_Unoccupied)
	{
		CHECK_INVALID_PTR(m_movement_component)
		m_movement_component->SetMovementMode(EMovementMode::MOVE_Falling);
	}

	if (m_attribute->IsAlive())
	{
		if (m_action_state == EActionState::EAS_Consume)
		{
			m_action_state = EActionState::EAS_Unoccupied;
			OnShowWeapon(true);
		}

		StopAllMontage();
		HitReact(Dir, AttackWeight);

		float percent = m_attribute->GetHealthPercent();
		if (percent < 0.5 && false == m_ui_mgr->IsActiveCallout(FName("Potion")))
		{
			m_ui_mgr->OpenTutorialCallout(FName("Potion"), EInputActionType::EIAT_Potion, false);
		}
	}
	else
	{
		if (EDeathPose::EDP_Alive != m_death_pose) return;
		FName section_name = FName("Death_01");

		UERGameInstance* instance = GetGameInstance<UERGameInstance>();
		CHECK_INVALID_PTR(instance)
		instance->SavePlayer(this);

		m_ui_mgr->CloseAllTutorial();
		m_ui_mgr->HideLockOn();

		m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_actor_target = nullptr;

		m_death_pose = EDeathPose::EDP_Death_Front;
		PlayMontageSection(m_montage_death, section_name);
		OnDeath(m_montage_death);

		UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
		CHECK_INVALID_PTR(observer_mgr)
		observer_mgr->TriggerDeathEvent(this);
		observer_mgr->TriggerEvent(EObserverEventType::EOET_BossGate);
	}

	if (nullptr != m_sound_hit) { UGameplayStatics::PlaySoundAtLocation(this, m_sound_hit, GetActorLocation()); }

	if (nullptr != m_particle_hit)
	{
		auto world = GetWorld();
		CHECK_INVALID_PTR(world)

		UGameplayStatics::SpawnEmitterAtLocation(world, m_particle_hit, ImpactPoint);
	}
}

float AC0000::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	m_attribute->ReceiveDamage(DamageAmount);

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
	PlayMontageSection(m_montage_guard, section_name);
}

void AC0000::OnAttackBlocked(const EAttackWeight& attack_weight)
{
	FName section_name =/* FName("Blocked_Normal")*/FName("Blocked_Strong");

	m_action_state = EActionState::EAS_BlockReact;

	PlayMontageSection(m_montage_blocked, section_name);

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
	m_attribute->RecoverHealth(250);

	PlayMontageSection(m_montage_potion_consume, FName("Consume_End"));

	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	if (m_ui_mgr->IsActiveCallout(FName("Potion")))
	{
		m_ui_mgr->CloseTutorialCallout();
	}
}

void AC0000::OnEndConsume()
{
	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(m_anim_instance);
	CHECK_INVALID_PTR(m_anim_instance)
	anim_instance->SetBlendPerBone(false);

	m_action_state = EActionState::EAS_Unoccupied;
}

void AC0000::OnUseStamina(const int32& amount_stamina)
{
	m_attribute->ConsumeStamina(amount_stamina);
}

void AC0000::OnEndExecution()
{
	m_actor_target = nullptr;
}

void AC0000::OnHideWeapon(const bool IsRightHand)
{
	CHECK_INVALID_PTR(m_equiped_weapon_R)
	if (m_equiped_weapon_R->IsUsingWeapon())
	{
		m_equiped_weapon_R->AttachMashToSocket(GetMesh(), FName("L_HipSocket"));
		m_equiped_weapon_R->SetWeaponUsing(false);
	}
}

void AC0000::OnShowWeapon(const bool IsRightHand)
{
	CHECK_INVALID_PTR(m_equiped_weapon_R)
	if (!m_equiped_weapon_R->IsUsingWeapon())
	{
		m_equiped_weapon_R->AttachMashToSocket(GetMesh(), FName("R_WeaponSocket"));
		m_equiped_weapon_R->SetWeaponUsing(true);
	}
}

const bool AC0000::IsCurrentActionState(const FString& action) const
{
	FString action_state = StaticEnum<EActionState_NPC>()->GetNameStringByValue(int64(m_action_state));
	UE_LOG(LogTemp, Warning, TEXT("IsCurrentActionState - ActionState : %s"), *action_state)

	if (action == "Attacking") { return m_action_state == EActionState::EAS_Attacking; }
	return false;
}

const bool AC0000::IsGuardState() const
{
	return m_action_state == EActionState::EAS_Guarding;
}

void AC0000::MoveForward(float value)
{
	if (nullptr == Controller || 0 == value) { return; }
	
	Movement(EAxis::X, value);
}

void AC0000::MoveSide(float value)
{
	if (nullptr == Controller || 0 == value) { return; }

	Movement(EAxis::Y, value);
}

void AC0000::Movement(const EAxis::Type axis, const float& value)
{
	if (false == IsInputPossible()) return;

	if (m_jump_state == EJumpState::EJS_Unoccupied)
	{
		m_anim_instance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		m_movement_component->bOrientRotationToMovement = true;
		m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}

	if (EActionState::EAS_Attacking != m_action_state)
	{
		bool is_key_down = m_player_controller->IsInputKeyDown(EKeys::RightMouseButton);
		if (false == is_key_down && m_action_state != EActionState::EAS_Consume) 
		{ 
			m_action_state = EActionState::EAS_Unoccupied; 
		}

		m_is_sprint = m_player_controller->IsInputKeyDown(EKeys::LeftShift) && (0 < m_attribute->GetStaminaCurrent());

		if (m_actor_target && m_lock_on_state == ELockOnState::ELOS_LockOn)
		{
			FVector direction	  = m_actor_target->GetActorLocation() - GetActorLocation();
			FVector direction_rotated = FRotationMatrix(direction.Rotation()).GetUnitAxis(axis);

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

			m_ground_state = EGroundState::EGS_Sprint;
			m_movement_component->MaxWalkSpeed = 600.f;

			UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(m_anim_instance);
			if (nullptr == anim_instance) return;

			const FVector last_input = GetLastMovementInputVector().GetSafeNormal2D();
			const FVector forward	 = GetActorForwardVector();

			double Theta = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(forward, last_input)));

			bool possible = anim_instance->IsEnableSprintTurn();
			if (FMath::Abs(Theta) >= 135.f && possible)
			{
				PlayMontageSection(m_montage_ground, FName("Forward_Turn"));
				m_action_state = EActionState::EAS_Turning;

				m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = false;
				anim_instance->ResetEnableSprintTurn();
			}
		}
		else
		{
			if (m_action_state != EActionState::EAS_Consume)
			{
				bool is_walk = m_player_controller->IsInputKeyDown(EKeys::LeftControl);

				m_ground_state = is_walk ? EGroundState::EGS_Walk : EGroundState::EGS_Jog;
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
		m_action_state	   = EActionState::EAS_Unoccupied;
		m_attack_name_prev = FName();
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
		false	== m_tl_cam_focus.IsPlaying() &&
		nullptr != m_actor_target)
	{
		FRotator rotator_direction	= UKismetMathLibrary::MakeRotFromX(m_actor_target->GetActorLocation() - GetActorLocation());
		FRotator rotator_controll	= m_player_controller->GetControlRotation();

		m_player_controller->SetControlRotation(FRotator(rotator_controll.Pitch, rotator_direction.Yaw, rotator_direction.Roll));
	}
	else if (m_direction_state == EDirectionState::EDS_DirectionOff)
	{
		AddControllerYawInput(value);
	}
}

void AC0000::ExitAction()
{
	CHECK_INVALID_SMART_PTR(m_ui_mgr)
	if (m_ui_mgr->IsEmptyUI())
	{
		m_ui_mgr->OpenExitPopUp(m_player_controller);
	}
}

void AC0000::Interaction()
{
	if (m_action_state == EActionState::EAS_Consume) return;

	if (!m_actor_vender && m_overlapping_item)
	{
		AItem* item = Cast<AItem>(m_overlapping_item);
		CHECK_INVALID_PTR(item)

		int32 idx = m_storage_mgr->HaveSpaceItem(item->GetItemObject());
		if (idx >= 0)
		{
			UItemObject* item_object = NewObject<UItemObject>();
			CHECK_INVALID_PTR(item_object)

			TObjectPtr<class UItemObject> data = item->GetItemObject();

			item_object->SetItemProperties(data->GetRowName(), *data->GetItemData(), data->GetItemClass(), false);
			item_object->SetCurrentHUD(EDragAndDropHUD::EDAD_Inventory);

			bool is_success_add = m_storage_mgr->TryAddItemAtIndex(item_object, idx);
			if (is_success_add)
			{
				item->Destroy();
				m_overlapping_item = nullptr;
			}
		}

	}
}

void AC0000::Equip()
{
	if (false == IsInputPossible()) return;

	const bool is_left	= m_player_controller->IsInputKeyDown(EKeys::Left);
	const bool is_right	= m_player_controller->IsInputKeyDown(EKeys::Right);

	CHECK_INVALID_PTR(m_montage_swap)
	FString section_name = "Swap";
	if	(is_left)	{ section_name += "_L"; m_swap_hand = EWeaponEquipHand::EWEH_Left; }
	else if (is_right)	{ section_name += "_R"; m_swap_hand = EWeaponEquipHand::EWEH_Right; }

	PlayMontageSection(m_montage_swap, FName(section_name));

	if (is_right)
	{
		bool is_using = m_equiped_weapon_R->IsUsingWeapon();
		m_equip_state = is_using ? EEquipState::EES_Unequipped : EEquipState::EES_EquippedOneHandedWeapon;
	}

	m_action_state	= EActionState::EAS_Equipping;
}

void AC0000::Attack()
{
	if (IsInputPossible()		==  false				||
	    m_action_state		==  EActionState::EAS_Guarding		||
	    m_action_state		==  EActionState::EAS_Consume		||
	    m_jump_state		>   EJumpState::EJS_Unoccupied  	||
	    m_equip_state		==  EEquipState::EES_Unequipped		||
	    m_enable_attack		==  false				||
	    0 >= m_attribute->GetStaminaCurrent()				||
	    m_equiped_weapon_R		==  nullptr				||
	    m_equiped_weapon_R->IsUsingWeapon() == false) return;
	
	if (m_attack_name_prev.IsNone() &&
		(m_is_enable_execution[(int32)EExecutionDirection::EED_Front] || 
		 m_is_enable_execution[(int32)EExecutionDirection::EED_Back])) 
	{
		AGameCharacter* target = Cast<AGameCharacter>(m_actor_execution_target);
		if (nullptr == target) return;

		m_enable_attack		= false;
		m_enable_input_movement = false;

		EGameDirection dir = m_is_enable_execution[(int32)EExecutionDirection::EED_Front] ? EGameDirection::EGD_Front : EGameDirection::EGD_Back;
		FString post_fix = (dir == EGameDirection::EGD_Back) ? "_Back" : "_Front";
		PlayMontageSection(m_montage_execution, FName("Execution" + post_fix));

		m_ui_mgr->HideLockOn();
		m_actor_target = target;

		m_lock_on_state = ELockOnState::ELOS_LockOff;
		m_movement_component->bOrientRotationToMovement = true;

		m_tl_cam_focus.PlayFromStart();
		if (dir == EGameDirection::EGD_Back) { m_tl_cam_execution_back.PlayFromStart(); }
		else				     { m_tl_cam_execution_front.PlayFromStart(); }

		target->TakeExecution(this, dir, m_equiped_weapon_R->GetWeaponDamage(EAttackType::EATKT_ParryAttack));

		m_action_state		= EActionState::EAS_Execution;
		m_direction_state	= EDirectionState::EDS_DirectionOn;
		m_is_not_damage_mod 	= true;
	}
	else
	{
		if (m_is_sprint)
		{
			m_attack_name_prev		= m_player_controller->IsInputKeyDown(EKeys::C) ? FName("Dash_Attack_Strong") : FName("Dash_Attack");
			m_attack_type_prev		= EAttackType::EATKT_DashAttack;

			m_action_state			= EActionState::EAS_Attacking;
			m_attack_strength		= EAttackStrength::EATKS_Normal;
			m_battle_pose			= EBattlePose::EBP_Attack;
			m_enable_input_movement		= false;
			m_enable_attack			= false;

			m_attribute->ConsumeStamina(30);

			PlayMontageSection(m_montage_attack, FName(m_attack_name_prev));

			m_equiped_weapon_R->SetAttackWeight(m_equip_state, EAttackStrength::EATKS_Normal, EAttackType::EATKT_Attack);
		}
		else if (m_jump_state > EJumpState::EJS_Unoccupied) 
		{
			m_attack_type_prev = EAttackType::EATKT_JumpAttack;
		}
		else
		{
			if (m_player_controller->IsInputKeyDown(EKeys::C)) 
			{
				FString attack_name = m_attack_name_prev.ToString();
				UE_LOG(LogTemp, Warning, TEXT("Prev Attack : %s"), *attack_name)

				if	(attack_name.Contains("Attack_Strong_01"))	m_attack_name_prev = FName("Attack_Strong_02");
				else if (attack_name.Contains("Attack_Strong_02"))	m_attack_name_prev = FName("Attack_Strong_01");
				else							m_attack_name_prev = FName("Attack_Strong_01");

				m_attack_strength = EAttackStrength::EATKS_Strong;
				m_equiped_weapon_R->SetAttackWeight(m_equip_state, EAttackStrength::EATKS_Strong, EAttackType::EATKT_Attack);
			}
			else 
			{
				if	(FName("Attack_01") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_02");
				else if (FName("Attack_02") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_03");
				else if (FName("Attack_03") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_04");
				else if (FName("Attack_04") == m_attack_name_prev)	m_attack_name_prev = FName("Attack_05");
				else							m_attack_name_prev = FName("Attack_01");

				m_enable_attack_short	= false;
				m_attack_strength	= EAttackStrength::EATKS_Normal;
				m_equiped_weapon_R->SetAttackWeight(m_equip_state, EAttackStrength::EATKS_Normal, EAttackType::EATKT_Attack);

				m_attribute->ConsumeStamina(20);
			}

			m_attack_type_prev		= EAttackType::EATKT_Attack;
			m_action_state			= EActionState::EAS_Attacking;
			m_battle_pose			= EBattlePose::EBP_Attack;
			m_enable_input_movement 	= false;
			m_enable_attack			= false;
			m_enable_input_attack_rotate 	= true;
			m_movement_component->bAllowPhysicsRotationDuringAnimRootMotion = true;

			PlayMontageSection(m_montage_attack, FName(m_attack_name_prev));
		}
	}
}

void AC0000::ShortAttack()
{
	if (m_enable_attack_short && !m_attack_name_prev.IsNone())
	{
		if	(FName("Attack_Strong_01") == m_attack_name_prev) m_attack_name_prev = FName("Attack_Strong_01_Short");
		else if (FName("Attack_Strong_02") == m_attack_name_prev) m_attack_name_prev = FName("Attack_Strong_02_Short");

		m_equiped_weapon_R->SetAttackWeight(m_equip_state, EAttackStrength::EATKS_Normal, EAttackType::EATKT_Attack);

		PlayMontageSection(m_montage_attack, FName(m_attack_name_prev));

		m_enable_attack_short = false;
	}
}

void AC0000::LockOn()
{
	if (m_lock_on_state == ELockOnState::ELOS_LockOn)
	{
		m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (m_actor_target)
		{
			m_ui_mgr->HideLockOn();
			m_actor_target = nullptr;
		}

		m_lock_on_state = ELockOnState::ELOS_LockOff;
	}
	else 
	{
		m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		if (m_actor_target && m_actor_target->IsAlive())
		{
			m_ui_mgr->ShowLockOn(m_player_controller, m_actor_target);
			m_tl_cam_focus.PlayFromStart();

			m_lock_on_state = ELockOnState::ELOS_LockOn;
		}
		else 
		{
			GetWorldTimerManager().SetTimer(m_timer_lockon, this, &AC0000::StartLockOnTimer, 1, false);
		}
	}
}

void AC0000::LockOnTargetChange(float value)
{
	if (m_lock_on_state != ELockOnState::ELOS_LockOn || m_lock_on_list.IsEmpty()) return;

	if (0 < value)
	{
		++m_lockon_index;
		if (m_lock_on_list.Num() <= m_lockon_index) m_lockon_index = 0;
	}
	else if (0 > value)
	{
		--m_lockon_index;
		if (0 > m_lockon_index) m_lockon_index = m_lock_on_list.Num() - 1;
	}
	
	if (0 != value)
	{
		if (false == m_lock_on_list.IsValidIndex(m_lockon_index)) m_lockon_index = 0;

		m_actor_target = m_lock_on_list[m_lockon_index];
		m_ui_mgr->ShowLockOn(m_player_controller, m_actor_target);
		m_tl_cam_focus.PlayFromStart();
	}
}

void AC0000::Dodge()
{
	if (!m_movement_component				||
	    false == IsInputPossible()				||
	    m_action_state == EActionState::EAS_Consume		||
	    m_action_state == EActionState::EAS_Attacking	||
	    (0 >= m_attribute->GetStaminaCurrent())) return;

	m_action_state = EActionState::EAS_Dodgeing;
	m_ground_state = EGroundState::EGS_Unoccupied;

	float		scale_x		= 0;
	float		scale_y		= 0;
	const bool	input_axis_x = IsInputKey(FName("MoveForward"), scale_x, true);
	const bool	input_axis_y = IsInputKey(FName("MoveSide"), scale_y, true);

	FString section_name = (input_axis_x || input_axis_y) ? FString("Dodge_Front") : FString("Dodge_Default");

	if (ELockOnState::ELOS_LockOn == m_lock_on_state)
	{
		if (input_axis_x && scale_x < 0) section_name = FString("Dodge_Back");
		else if (input_axis_y)
		{
			if	(scale_y > 0) section_name = FString("Dodge_Right");
			else if (scale_y < 0) section_name = FString("Dodge_Left");
		}
	}

	m_attribute->ConsumeStamina(40.f);

	PlayMontageSection(m_montage_dodge, FName(section_name));
}

void AC0000::Guard()
{
	IWeapon_Interface* shield = Cast<IWeapon_Interface>(m_equiped_weapon_L);

	if (IsInputPossible()	== false			||
	    m_action_state	== EActionState::EAS_Attacking  ||
	    m_action_state	== EActionState::EAS_Consume	||
	    m_jump_state	!= EJumpState::EJS_Unoccupied	||
	    !shield) return;


	CHECK_INVALID_SMART_PTR(m_player_controller)
	bool is_key_down = m_player_controller->IsInputKeyDown(EKeys::RightMouseButton);
	if (is_key_down) 
	{ 
		m_action_state = EActionState::EAS_Guarding; 
	}

	shield->SetShieldMod(is_key_down);
	shield->SetWeaponCollision(is_key_down ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	FString section_name = is_key_down ? FString("GuardStart") : FString("GuardEnd");

	if (false == IsInputAxisActionKey())
	{
		section_name += FString("_Idle");
		PlayMontageSection(m_montage_guard, FName(section_name));
	}
}

void AC0000::Inventory()
{
	CHECK_INVALID_SMART_PTR(m_storage_mgr)
	m_storage_mgr->OpenInventory(this, m_player_controller);
}

void AC0000::Parry()
{
	if (EActionState::EAS_Parry   == m_action_state || 
	    EActionState::EAS_Consume == m_action_state) return;

	IWeapon_Interface* shield = Cast<IWeapon_Interface>(m_equiped_weapon_L);
	CHECK_INVALID_PTR(shield)

	m_action_state = EActionState::EAS_Parry;
	shield->SetWeaponCollision(ECollisionEnabled::NoCollision);

	PlayMontageSection(m_montage_guard, FName("Guard_Parry"));
}

void AC0000::Consume()
{
	if (m_action_state != EActionState::EAS_Unoccupied ||
	    m_jump_state > EJumpState::EJS_Unoccupied) return;

	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(m_anim_instance);
	CHECK_INVALID_PTR(anim_instance)

	anim_instance->SetBlendPerBone(true);
	m_action_state = EActionState::EAS_Consume;
	m_swap_hand	   = EWeaponEquipHand::EWEH_Right;

	FName section_name = (m_cur_potion > 0) ? FName("Consume_Start") : FName("Consume_Fail");
	PlayMontageSection(m_montage_potion_consume, section_name);

	double standard		 = m_cur_potion * 0.5;
	m_cur_potion		 = (m_cur_potion > 0) ? (m_cur_potion - 1) : 0;
	EPotionSizeType size = (m_cur_potion > (standard)) ? EPotionSizeType::EPST_Half : EPotionSizeType::EPST_Less;
	if (m_cur_potion <= 0) { size = EPotionSizeType::EPST_Empty; }
	m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Bottom, nullptr, size);
}

void AC0000::StartJump()
{
	if (EActionState::EAS_Attacking ==  m_action_state ||
	    EActionState::EAS_Equipping ==  m_action_state ||
	    EActionState::EAS_Dodgeing  ==  m_action_state ||
	    EActionState::EAS_Turning   ==  m_action_state ||
	    EActionState::EAS_Consume   ==  m_action_state ||
	    EJumpState::EJS_JumpStart   ==  m_jump_state) return;

	if (!m_movement_component || !CanJump()) return;

	m_jump_state	= EJumpState::EJS_JumpStart;
	float speed_cur = m_movement_component->Velocity.Size2D();

	FString section_name = (150.f <= speed_cur) ? FString("Jump_Front") : FString("Jump_Standing");

	if (ELockOnState::ELOS_LockOn == m_lock_on_state)
	{
		float scale = 0;
		if (IsInputKey(FName("MoveForward"), scale, true))
		{
			if (scale < 0)		section_name = FString("Jump_Back");
		}
		else if (IsInputKey(FName("MoveSide"), scale, true))
		{
			if	(scale > 0)	section_name = FString("Jump_Right");
			else if (scale < 0)	section_name = FString("Jump_Left");
		}
	}

	if (300.f <= speed_cur) section_name += FString("_Medium");

	PlayMontageSection(m_montage_jump, FName(section_name));
}

void AC0000::OnDetectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGameCharacter* character = Cast<AGameCharacter>(OtherActor);
	if (!character || this == character || character->ActorHasTag(FName("Vender"))) return;

	if (IsInViewport(character->GetActorLocation()) && character->IsAlive())
	{
		m_lock_on_list.AddUnique(character);

		if (0 >= m_lock_on_list.Num()) return;

		FVector user_location = GetActorLocation();
		Algo::Sort(m_lock_on_list, [user_location](AGameCharacter* const left, AGameCharacter* const right)
			{
				return (left->GetActorLocation() - user_location).Size() < (right->GetActorLocation() - user_location).Size();
			});

		if (nullptr == m_actor_target)
		{
			m_lock_on_state = ELockOnState::ELOS_LockOn;
			m_actor_target  = m_lock_on_list[0];
			m_movement_component->bOrientRotationToMovement = false;

			m_ui_mgr->ShowLockOn(m_player_controller, m_actor_target);
			m_tl_cam_focus.PlayFromStart();
		}
	}
}

void AC0000::OnDetectEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGameCharacter* character = Cast<AGameCharacter>(OtherActor);
	if (!character || this == character) return;

	for (AGameCharacter* const target : m_lock_on_list)
	{
		if (character == target)
		{
			if (m_actor_target == target) 
			{
				m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				m_lock_on_state = ELockOnState::ELOS_LockOff;
				m_movement_component->bOrientRotationToMovement = true;

				if (nullptr == m_actor_target) return;
				m_ui_mgr->HideLockOn();
				m_actor_target = nullptr;
			}

			m_lock_on_list.Remove(character);
			return;
		}
	}

	m_lock_on_list.Shrink();
}

bool AC0000::IsInViewport(const FVector& target_location)
{
	FVector2D location_viewport;
	m_player_controller->ProjectWorldLocationToScreen(target_location, location_viewport);

	int32 viewport_x, viewport_y;
	m_player_controller->GetViewportSize(viewport_x, viewport_y);

	return ((location_viewport.X > 0 && location_viewport.Y > 0) &&
		(location_viewport.X < viewport_x && location_viewport.Y < viewport_y));
}

void AC0000::UninitAndDestroy()
{
	Super::UninitAndDestroy();

	AERGameMode* mode = Cast<AERGameMode>(UGameplayStatics::GetGameMode(this));
	CHECK_INVALID_PTR(mode)
	mode->RequestRestartPlayer(m_player_controller.Get());
}

void AC0000::SetLandingFinish()
{
	m_jump_state   = EJumpState::EJS_Unoccupied; 
	m_action_state = EActionState::EAS_Unoccupied;
}

void AC0000::ReleaseWeapon(const bool IsRight)
{
	if (IsRight)
	{
		if (m_equiped_weapon_R) { m_equiped_weapon_R->Destroy(); }
		m_equiped_weapon_R = nullptr;

		m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Right);
	}
	else
	{
		if (m_equiped_weapon_L) { m_equiped_weapon_L->Destroy(); }
		m_equiped_weapon_L = nullptr;

		m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Left);
	}
}

void AC0000::Swap_Weapon(const EWeaponEquipHand& hand_type)
{
	if (hand_type == EWeaponEquipHand::EWEH_Right)
	{
		if (m_ui_mgr->IsActiveCallout(FName("Swap"))) { m_ui_mgr->CloseTutorialCallout(); }

		CHECK_INVALID_PTR(m_equiped_weapon_R)
		if (m_equiped_weapon_R->IsUsingWeapon())
		{
			m_equiped_weapon_R->AttachMashToSocket(GetMesh(), FName("L_HipSocket"));
			m_equiped_weapon_R->SetWeaponUsing(false);

			m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Right);
		}
		else
		{
			m_equiped_weapon_R->AttachMashToSocket(GetMesh(), FName("R_WeaponSocket"));
			m_equiped_weapon_R->SetWeaponUsing(true);

			m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Right, m_equiped_weapon_R->GetItemObject());
		}
	}
	else
	{
		CHECK_INVALID_PTR(m_equiped_weapon_L)
		if (m_equiped_weapon_L->IsUsingWeapon())
		{
			m_equiped_weapon_L->AttachMashToSocket(GetMesh(), FName("Spine1_MantleSocket"));
			m_equiped_weapon_L->SetWeaponUsing(false);

			m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Left);
		}
		else
		{
			m_equiped_weapon_L->AttachMashToSocket(GetMesh(), FName("L_ShieldSocket"));
			m_equiped_weapon_L->SetWeaponUsing(true);

			m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Left, m_equiped_weapon_L->GetItemObject());
		}
	}
}

void AC0000::GetJumpDirectionLength(const FVector& apex_location)
{
	const FVector direction	= apex_location - m_location_jump_start;
	const double  length	= direction.Size2D();

	AddMovementInput(direction, length, true);
}

const bool AC0000::IsInputKey(const FName& Input_name, float& scale, const bool& is_axis)
{
	if (nullptr == m_player_controller) { return false; }

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
			if (m_player_controller->IsInputKeyDown(key.Key)) { return true; }
		}
	}

	return false;
}

const bool AC0000::IsInputAxisActionKey()
{
	if (nullptr == m_player_controller) return false;
	
	float scale;
	if (IsInputKey(FName("MoveForward"), scale, true)	||
	    IsInputKey(FName("MoveSide"), scale, true)		||
	    IsInputKey(FName("Attack"), scale, false)		||
	    IsInputKey(FName("Jump"), scale, true)		||
	    IsInputKey(FName("Dodge"), scale, true)) return true;

	return false;
}

const bool AC0000::IsInputKeyByFKey(const FKey& key)
{
	if (nullptr == m_player_controller) { return false; }

	return m_player_controller->IsInputKeyDown(key);
}

const bool AC0000::IsFullHealthPoint()
{
	return  m_attribute->GetHealthCurrent() == m_attribute->GetHealthMax();
}

void AC0000::OnEndTurn()
{
	m_action_state = EActionState::EAS_Unoccupied;

	if (m_is_sprint)
	{
		CHECK_INVALID_PTR(m_anim_instance);
		m_anim_instance->StopAllMontages(0.5f);
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

void AC0000::OnDiscovered(const FString& Name)
{
	PlayMontageSection(m_montage_discover, FName("GraceDiscover"));
	CheckPointSave(Name);
}

void AC0000::OnEnterBossRoom()
{
	DisableInput(m_player_controller.Get());
	PlayMontageSection(m_montage_to_boss, FName("EnterBoss"));
}

void AC0000::OnRest(const FString& Name)
{
	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(m_anim_instance);
	anim_instance->SetRequestMontage(m_montage_sit);
	PlayMontageSection(m_montage_sit, FName("SitStart"));

	CheckPointSave(Name);
}

void AC0000::OnEndRest()
{
	UC0000_AnimInstance* anim_instance = Cast<UC0000_AnimInstance>(m_anim_instance);
	anim_instance->SetRequestMontage(m_montage_sit);
	PlayMontageSection(m_montage_sit, FName("SitEnd"));

	m_action_state = EActionState::EAS_Sit;
	m_tl_cam_rest.PlayFromStart();
}

void AC0000::OnRecover()
{
	if (m_attribute->GetHealthMax() == m_attribute->GetHealthCurrent() && m_cur_potion == m_max_potion)
	{
		m_ui_mgr->OpenSubNotify(FText::FromString(TEXT("체력과 포션이 모두 가득 찼습니다.")));
	}
	else
	{
		if (m_ui_mgr->IsActiveCallout(FName("Potion"))) { m_ui_mgr->CloseTutorialCallout(); }

		UParticleSystem* particle_system = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/PlayerBuffs/P_CarriedBuff_Received_Red.P_CarriedBuff_Received_Red'"));
		if (!particle_system) return;

		UGameplayStatics::SpawnEmitterAttached(
			particle_system,
			RootComponent,
			FName("Spine"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget
		);

		m_attribute->ResetHealthToFull();

		m_cur_potion = m_max_potion;
		m_hud_component->SetQuickSlotItem(EQuickSlotType::EQST_Bottom, nullptr, EPotionSizeType::EPST_Full);

		USoundBase* recover = LoadObject<USoundBase>(nullptr, TEXT("MetaSoundSource'/Game/EldenRing/Sounds/Items/Potion/sfx_Potion.sfx_Potion'"));
		CHECK_INVALID_PTR(recover)

		UAudioComponent* recover_component = UGameplayStatics::CreateSound2D(GetWorld(), recover);
		CHECK_INVALID_PTR(recover_component)

		recover_component->bAutoDestroy = true;
		recover_component->bAllowSpatialization = false;
		recover_component->Play();
	}
}

void AC0000::OnCameraFocusRotate(float curve_value)
{
	CameraDirecting(EERCameraDirect::ECD_Focus, curve_value);
}

void AC0000::OnCameraExecutionFront(float curve_value)
{
	CameraDirecting(EERCameraDirect::ECD_Front, curve_value);

	//if (rotation_calc == rotator_goal) { m_timeline_camera_direction.Stop(); }
}

void AC0000::OnCameraExecutionBack(float curve_value)
{
	CameraDirecting(EERCameraDirect::ECD_Back, curve_value);
}

void AC0000::OnCameraRest(float curve_value)
{
	m_spring_arm->TargetArmLength = m_spring_arm_length - (curve_value * 150);

	if (m_spring_arm_length == m_spring_arm->TargetArmLength) { m_tl_cam_rest.Stop(); }
}

void AC0000::OnCameraShift(const float Distance, const float PitchOffset, const float YawOffset, const float RollOffset)
{
	FRotator rotator_current = m_player_controller->GetControlRotation();
	FRotator rotator_goal = FRotator(rotator_current.Pitch + PitchOffset, rotator_current.Yaw + YawOffset, rotator_current.Roll + RollOffset);

	m_spring_arm->TargetArmLength = Distance;
	m_player_controller->SetControlRotation(FRotator(rotator_goal.Pitch, rotator_goal.Yaw, rotator_goal.Roll));
}

void AC0000::CameraDirecting(const EERCameraDirect& type, const float& curve_value)
{
	FTimeline* timeline = nullptr;
	switch (type)
	{
		case EERCameraDirect::ECD_Focus: timeline = &m_tl_cam_focus;		break;
		case EERCameraDirect::ECD_Front: timeline = &m_tl_cam_execution_front;	break;
		case EERCameraDirect::ECD_Back:  timeline = &m_tl_cam_execution_back;	break;
	}

	if (!m_actor_target) { timeline->Stop(); return; }

	if (type != EERCameraDirect::ECD_Focus) { m_spring_arm->TargetArmLength = m_spring_arm_length - (curve_value * 150); }

	FRotator rotator_current   = m_player_controller->GetControlRotation();
	FRotator rotator_direction = UKismetMathLibrary::MakeRotFromX(m_actor_target->GetActorLocation() - GetActorLocation());

	FRotator rotator_goal	   = FRotator(type == EERCameraDirect::ECD_Focus ? rotator_current.Pitch : 340.f, rotator_direction.Yaw, rotator_direction.Roll);
	FRotator rotation_calc     = UKismetMathLibrary::RInterpTo(m_player_controller->GetControlRotation(), rotator_goal, curve_value, 10);

	m_player_controller->SetControlRotation(FRotator(rotator_current.Pitch, rotation_calc.Yaw, rotation_calc.Roll));

	if (type == EERCameraDirect::ECD_Focus && rotation_calc == rotator_goal) { m_tl_cam_focus.Stop(); }
}

void AC0000::SetWeaponCollision(ECollisionEnabled::Type type)
{
	CHECK_INVALID_PTR(m_equiped_weapon_R)
	IWeapon_Interface* weapon = Cast<IWeapon_Interface>(m_equiped_weapon_R);
	CHECK_INVALID_PTR(weapon)
	weapon->SetWeaponCollision(type);
}

void AC0000::SetGroundUnoccupied()
{
	m_ground_state = EGroundState::EGS_Unoccupied;

	float percent = m_attribute->GetStaminaPercent();
	if (1 == percent) { return; }
}

void AC0000::CheckPointSave(const FString& Name)
{
	UERGameInstance* instance = Cast<UERGameInstance>(UGameplayStatics::GetGameInstance(this));
	CHECK_INVALID_PTR(instance)

	TArray<AActor*> points;
	UGameplayStatics::GetAllActorsWithTag(this, FName("SpawnPoint"), points);

	if (points.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnPoint is not exist in level"))
		return;
	}

	for (AActor* const point : points)
	{
		AObjectSpawnPoint* spawn_point = Cast<AObjectSpawnPoint>(point);
		if (!spawn_point) continue;

		if (spawn_point->GetLocationName() == Name)
		{
			m_start_transform = spawn_point->GetActorTransform();
			break;
		}
	}

	instance->SavePlayer(this);
}

void AC0000::GameStartAction()
{
	UE_LOG(LogTemp, Warning, TEXT("[Gameplay Start]"))

	PlayMontageSection(m_montage_sit, FName("SitEnd"));
	float duration = 0;

	TArray<FCompositeSection> sections = m_montage_sit->CompositeSections;
	for (const auto& section : sections)
	{
		if (section.SectionName == FName("SitEnd"))
		{
			const UAnimSequenceBase* sequence = section.GetLinkedSequence();
			if (!sequence) break;

			duration = sequence->GetPlayLength();
			break;
		}
	}

	FLatentActionInfo callback;
	callback.CallbackTarget = this;
	callback.Linkage = 0;
	callback.UUID = FGuid::NewGuid().A;
	callback.ExecutionFunction = FName("AllowEnableInput");

	UKismetSystemLibrary::Delay(this, duration, callback);
}

void AC0000::StartLockOnTimer()
{
	if (!m_actor_target) 
	{
		GetWorldTimerManager().ClearTimer(m_timer_lockon);
		m_sphere_detect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AC0000::AllowEnableInput()
{
	EnableInput(m_player_controller.Get());
}

