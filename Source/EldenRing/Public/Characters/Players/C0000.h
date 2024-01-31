// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GameCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Characters/Players/Player_Interface.h"
#include "Components/TimelineComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Common/Hit/HitInterface.h"
#include "Items/Equipment/EquipmentTypes.h"
#include "GameData/SaveDatas.h"
#include "GameFramework/Character.h"
#include "Camera/ERCameraTypes.h"
#include "C0000.generated.h"

class UAnimMontage;
class UInventoryComponent;
class AItem;
class AVender;

UCLASS()
class ELDENRING_API AC0000 : public AGameCharacter, public IPlayer_Interface
{
	GENERATED_BODY()

public:
	AC0000();
	virtual void Tick(float DeltaTime) override final;

	virtual void SetDefaultItems() override;
	void AdjustItemOption(const bool& IsEquip, const TObjectPtr<UItemObject> ItemObject);

	virtual void NotifyControllerChanged() override;
	virtual void SetWeaponCollision(ECollisionEnabled::Type type) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight) override final;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;			 
	void SetWeapon(const bool& IsRight, AWeapon_Actor* const Weapon);
	
	FORCEINLINE virtual class UGameHUDComponent* const		GetHUDComponent() const override final	{ return m_hud_component; };
	FORCEINLINE virtual const TObjectPtr<class APlayerController>	GetControllerComponent() override final { return Cast<APlayerController>(Controller); };

	virtual void OnAttackDefended(const EAttackWeight& attack_weight) override final;
	virtual void OnAttackBlocked(const EAttackWeight& attack_weight) override final;
	virtual void OnReactEnd() override final;
	virtual void OnCameraShakeIn();
	virtual void OnCameraShakeOut();
	virtual void OnSuccessConsume();
	virtual void OnEndConsume();
	virtual void OnUseStamina(const int32& amount_stamina);
	virtual void OnEndExecution();
	virtual void OnHideWeapon(const bool IsRightHand);
	virtual void OnShowWeapon(const bool IsRightHand);
				 
	FORCEINLINE virtual const bool IsAttacking() const override final { return m_action_state == EActionState::EAS_Attacking; }
	virtual const bool IsCurrentActionState(const FString& action) const override final;
	virtual const bool IsGuardState() const override final;

	FORCEINLINE EActionState			GetActionState()		{ return m_action_state; }
	FORCEINLINE EJumpState				GetJumpState()			{ return m_jump_state; }
	FORCEINLINE EGroundState			GetGroundState()		{ return m_ground_state; }
	FORCEINLINE EGameDirection			GetCharacterDirection()		{ return m_game_direction; }
	FORCEINLINE ELockOnState			GetLockOnState()		{ return m_lock_on_state; }
	FORCEINLINE AActor* const			GetTarget()			{ return m_actor_target; }
	FORCEINLINE AVender*const			GetTradePartner()		{ return m_actor_vender; }
	FORCEINLINE bool				GetInputEnableMovement()	{ return m_enable_input_movement; }
	FORCEINLINE bool				GetInputEnableAttackRotate()	{ return m_enable_input_attack_rotate; }
	FORCEINLINE const EWeaponEquipHand&		GetSwapHand()			{ return m_swap_hand; }
				 
	FORCEINLINE void SetOverlappingItem(AItem* item)			{ m_overlapping_item		= item; }
	FORCEINLINE void SetJumpStartLocation(const FVector& start_location)	{ m_location_jump_start		= start_location; }
	FORCEINLINE void SetInputMovementEnable(const bool& enable)		{ m_enable_input_movement 	= enable; }
	FORCEINLINE void SetInputAttackRotateEnable(const bool& enable)		{ m_enable_input_attack_rotate	= enable; }
	FORCEINLINE void SetActionUnoccupied()					{ m_action_state 		= EActionState::EAS_Unoccupied; }
	FORCEINLINE void SetAttackEnable(const bool& enable)			{ m_enable_attack		= enable; }
	FORCEINLINE void SetShortAttackEnable(const bool& enable)		{ m_enable_attack_short		= enable; }
	FORCEINLINE void SetTradePartner(AVender* const vender)			{ m_actor_vender		= vender; }

protected:		 
	virtual void BeginPlay() override;
	virtual void UninitAndDestroy() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight) override final;
	virtual void FadeOutCharacter(const float Duration) override final;
	virtual const float UpdateFadeOut(const float& Value) override final;
	virtual void FinishFadeOut(class UNiagaraComponent* PSystem) override final;
	virtual bool IsInputPossible();
	virtual void GameStartAction() override final;

	void MoveForward(float value);
	void MoveSide(float value);
	void Movement(const EAxis::Type axis, const float& value);
	void LookUp(float value);
	void Turn(float value);
	void LockOnTargetChange(float value);

	void ExitAction();
	void Interaction();
	void Equip();
	void Attack();
	void ShortAttack();
	void LockOn();
	void Dodge();
	void Guard();
	void Inventory();
	void Parry();
	void Consume();

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void OnDetectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsInViewport(const FVector& target_location);

public:
	void SetLandingFinish();

	void ReleaseWeapon(const bool IsRight);

	void Swap_Weapon(const EWeaponEquipHand& hand_type);
	void GetJumpDirectionLength(const FVector& apex_location);
	void OnEndTurn();
	void OnEndAttack();
	void OnEndDirection();
	void OnDodgeEnd();
	void OnGuardEnd();
	void OnStartKnockback();
	void OnEndKnockback();
	void OnDiscovered(const FString& Name);
	void OnEnterBossRoom();
	void OnRest(const FString& Name);
	void OnEndRest();
	void OnRecover();

	UFUNCTION()
	void OnCameraFocusRotate(float curve_value);
	UFUNCTION()
	void OnCameraExecutionFront(float curve_value);
	UFUNCTION()
	void OnCameraExecutionBack(float curve_value);

	UFUNCTION()
	void OnCameraRest(float curve_value);
	void OnCameraShift(const float Distance, const float PitchOffset, const float YawOffset, const float RollOffset);

	void CameraDirecting(const EERCameraDirect& type, const float& curve_value);

	const bool IsInputKey(const FName& Input_name, float& scale, const bool& is_axis);
	const bool IsInputAxisActionKey();
	const bool IsInputKeyByFKey(const FKey& key);
	const bool IsFullHealthPoint();

	void SetGroundUnoccupied();

private:
	void CheckPointSave(const FString& Name);

	UFUNCTION()
	void StartLockOnTimer();
	
	UFUNCTION()
	void AllowEnableInput();

private:
#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	UAnimMontage* m_montage_hit_medium;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	UAnimMontage* m_montage_hit_ultra;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_swap;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_jump;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_land;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_land_jog;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_dodge;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_ground;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_sit;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_discover;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_to_boss;
#pragma endregion

#pragma region Component
	UPROPERTY()
	TWeakObjectPtr<class APlayerController> m_player_controller;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> m_spring_arm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> m_view_camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> m_sphere_detect;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> m_movement_component;
	
	UPROPERTY()
	TObjectPtr<class UGameHUDComponent> m_hud_component;
#pragma endregion

#pragma region Camera
	UPROPERTY(EditDefaultsOnly, Category = "Camera Shake : Execution")
	TSubclassOf<class UExecutionCameraShake> m_class_camera_shake_execution_in;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Shake : Execution")
	TSubclassOf<class UExecutionCameraShake> m_class_camera_shake_execution_out;
#pragma endregion

#pragma region Actors
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> m_overlapping_item;

	TArray<TObjectPtr<AGameCharacter>> m_lock_on_list;

	TObjectPtr<AVender> m_actor_vender = nullptr;
#pragma endregion

#pragma region Attack
	FName			m_attack_name_prev;
	EAttackType		m_attack_type_prev	= EAttackType::EATKT_None;
	EAttackStrength 	m_attack_strength	= EAttackStrength::EATKS_None;
	EBattlePose		m_battle_pose		= EBattlePose::EBP_Unoccupied;
#pragma endregion

#pragma region Equipments
	TArray<TArray<class UMaterialInstanceDynamic*>> m_equipment_materials;
#pragma endregion

#pragma region States
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState		m_action_state		= EActionState::EAS_Unoccupied;
	EJumpState		m_jump_state		= EJumpState::EJS_Unoccupied;
	EGroundState		m_ground_state		= EGroundState::EGS_Unoccupied;
	ELockOnState		m_lock_on_state		= ELockOnState::ELOS_LockOff;
	EDirectionState		m_direction_state	= EDirectionState::EDS_DirectionOff;
	EGameDirection		m_game_direction	= EGameDirection::EGD_None;
	EWeaponEquipHand	m_swap_hand		= EWeaponEquipHand::EWEH_Right;
#pragma endregion

#pragma region TimeLine
	FTimeline m_tl_cam_focus;
	FTimeline m_tl_cam_execution_front;
	FTimeline m_tl_cam_execution_back;
	FTimeline m_tl_cam_rest;
#pragma endregion

#pragma region Curves
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> m_curve_camera_focus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> m_curve_camera_execution_front;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> m_curve_camera_execution_back;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> m_curve_camera_rest_end;
#pragma endregion

#pragma region Timer
	FTimerHandle m_timer_hit_react;
	FTimerHandle m_timer_lockon;
#pragma endregion

#pragma region Values
	bool m_is_sprint = false;
	bool m_is_guard;
	bool m_enable_sprint_turn;
	bool m_enable_attack;
	bool m_enable_attack_short;
	bool m_enable_input_movement = false;
	bool m_enable_input_attack_rotate = false;

	float m_spring_arm_length;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Potion", meta = (AllowPrivateAccess = "true"))
	int32 m_max_potion;
	int32 m_cur_potion;

	FVector	m_location_jump_start = FVector::ZeroVector;
#pragma endregion

#pragma region SubSystem
	UPROPERTY()
	TWeakObjectPtr<class UStorageManager>		m_storage_mgr;
	UPROPERTY()
	TWeakObjectPtr<class UEquipmentManager>		m_equipment_mgr;
	UPROPERTY()
	TWeakObjectPtr<class UInteractionManager>	m_interact_mgr;
	UPROPERTY()
	TWeakObjectPtr<class UUIManager>		m_ui_mgr;
#pragma endregion

#pragma region Starter Items
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FName m_name_weapon_R;

	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FName m_name_weapon_L;

	UPROPERTY(EditDefaultsOnly, Category = "Starter Items", meta = (PrivateAllowAccess = "true"))
	FName m_starter_head;

	UPROPERTY(EditDefaultsOnly, Category = "Starter Items", meta = (PrivateAllowAccess = "true"))
	FName m_starter_body;

	UPROPERTY(EditDefaultsOnly, Category = "Starter Items", meta = (PrivateAllowAccess = "true"))
	FName m_starter_arms;

	UPROPERTY(EditDefaultsOnly, Category = "Starter Items", meta = (PrivateAllowAccess = "true"))
	FName m_starter_legs;
#pragma endregion

#pragma region FX
	UPROPERTY()
	UParticleSystemComponent* m_particle_buff;
#pragma endregion
};
