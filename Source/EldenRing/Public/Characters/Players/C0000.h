// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GameCharacter.h"
#include "GameFramework/Character.h"
#include "../CharacterTypes.h"
#include "../../Items/Equipment/EquipmentTypes.h"
#include "../../Interfaces/HitInterface.h"
#include "Components/PrimitiveComponent.h"
#include "C0000.generated.h"

class UCharacterMovementComponent;
class APlayerController;
class USpringArmComponent;
class UCameraComponent;
class UExecutionCameraShake;
class UGameHUD;
class UAnimMontage;
class USphereComponent;
class UInventoryComponent;
class AItem;
class UItemObject;

struct FFOscillator;

UCLASS()
class BASIC_API AC0000 : public AGameCharacter
{
	GENERATED_BODY()

public:
	AC0000();

	virtual void Tick(float DeltaTime) override;
	virtual void SetWeaponCollision(ECollisionEnabled::Type type) override final;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override final;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight) override final;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override final;

	virtual void OnAttackDefended(const EAttackWeight& attack_weight) override final;
	virtual void OnAttackBlocked(const EAttackWeight& attack_weight) override final;
	virtual void OnReactEnd() override final;

	virtual bool IsCurrentActionState(const FString& action) override final;
	virtual bool IsGuardState() override final;

protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

	virtual void HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight) override final;

	bool IsInputPossible();

	void MoveForward(float value);
	void MoveSide(float value);
	void Movement(const EAxis::Type axis, const float& value);
	void LookUp(float value);
	void Turn(float value);
	void LockOnTargetChange(float value);

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

	void MeshActivate(UItemObject* const item_object, const bool& is_show);
	bool IsInViewport(const FVector& target_location);

public:
	FORCEINLINE EActionState			GetActionState()		{ return m_action_state; }
	FORCEINLINE EEquipState				GetEquipState()			{ return m_equip_state; }
	FORCEINLINE EJumpState				GetJumpState()			{ return m_jump_state; }
	FORCEINLINE EGroundState			GetGroundState()		{ return m_ground_state; }
	FORCEINLINE EGameDirection			GetCharacterDirection()		{ return m_game_direction; }
	FORCEINLINE ELockOnState			GetLockOnState()		{ return m_lock_on_state; }
	FORCEINLINE AActor*				GetTarget()			{ return m_actor_target; }
	FORCEINLINE UInventoryComponent* const		GetInventoryComponent()		{ return m_inventory_component; }
	FORCEINLINE bool				GetInputEnableMovement()	{ return m_enable_input_movement; }
	FORCEINLINE bool				GetInputEnableAttackRotate()	{ return m_enable_input_attack_rotate; }
	FORCEINLINE int32				GetTotalGold()			{ return m_total_gold; }
	FORCEINLINE const EWeaponEquipHand&		GetSwapHand()			{ return m_swap_hand; }
	FORCEINLINE const bool&				IsNotDamageMod()		{ return m_is_not_damage_mod; }

	FORCEINLINE void SetOverlappingItem(AItem* item)				{ m_overlapping_item		= item; }
	FORCEINLINE void SetJumpStartLocation(const FVector& start_location)		{ m_location_jump_start		= start_location; }
	FORCEINLINE void SetInputMovementEnable(const bool& enable)			{ m_enable_input_movement	= enable; }
	FORCEINLINE void SetInputAttackRotateEnable(const bool& enable)			{ m_enable_input_attack_rotate	= enable; }
	FORCEINLINE void SetLandingFinish()						{ m_jump_state			= EJumpState::EJS_Unoccupied; }
	FORCEINLINE void SetActionUnoccupied()						{ m_action_state		= EActionState::EAS_Unoccupied; }
	FORCEINLINE void SetAttackEnable(const bool& enable)				{ m_enable_attack		= enable; }
	FORCEINLINE void SetShortAttackEnable(const bool& enable)			{ m_enable_attack_short		= enable; }

	void EquipArmorFromInventory(UItemObject* const item_object);
	void EquipWeaponFromInventory(UItemObject* const item_object, const bool& is_right_hand = true);
	void UnequipArmor(const EEquipmentType& equip_type);
	void UnequipWeapon(const EEquipmentType& equip_type, const bool& is_right_hand = true);

	void Swap_Weapon(const EWeaponEquipHand& hand_type);
	void GetJumpDirectionLength(const FVector& apex_location);
	void OnEndTurn();
	void OnEndAttack();
	void OnEndDirection();
	void OnDodgeEnd();
	void OnGuardEnd();
	void OnStartKnockback();
	void OnEndKnockback();

	UFUNCTION()
	void OnCameraFocusRotate(float curve_value);
	UFUNCTION()
	void OnCameraExecutionFront(float curve_value);
	UFUNCTION()
	void OnCameraExecutionBack(float curve_value);

	void OnCameraShakeIn();
	void OnCameraShakeOut();

	bool IsInputKey(const FName& Input_name, float& scale, const bool& is_axis);
	bool IsInputAxisActionKey();
	bool IsInputKeyByFKey(const FKey& key);

	void SetGroundUnoccupied();
	void SetGold(const int32& gold);

	void OnSuccessConsume();
	void OnEndConsume();
	void OnUseStamina(const int32& amount_stamina);

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
#pragma endregion

#pragma region Component
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent*		m_spring_arm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent*		m_view_camera;

	UPROPERTY(VisibleAnywhere)
	USphereComponent*		m_sphere_detect;

	UCharacterMovementComponent*	m_movement_component;
	APlayerController*		m_player_controller;
#pragma endregion

#pragma region Camera
	UPROPERTY(EditDefaultsOnly, Category = "Camera Shake : Execution")
	TSubclassOf<UExecutionCameraShake>	m_class_camera_shake_execution_in;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Shake : Execution")
	TSubclassOf<UExecutionCameraShake>	m_class_camera_shake_execution_out;
#pragma endregion

#pragma region Actors
	UPROPERTY(VisibleInstanceOnly)
	AItem* m_overlapping_item;

	TArray<AGameCharacter*> m_lock_on_list;
#pragma endregion

#pragma region Attack
	FName			m_attack_name_prev 	= "";
	EAttackType		m_attack_type_prev	= EAttackType::EATKT_None;
	EAttackStrength 	m_attack_strength	= EAttackStrength::EATKS_None;
	EBattlePose		m_battle_pose		= EBattlePose::EBP_Unoccupied;
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

#pragma region HUD
	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* m_inventory_component;

	UPROPERTY(VisibleAnywhere)
	UGameHUD* m_hud_overlay;
#pragma endregion

#pragma region TimeLine
	FTimeline m_timeline_camera_focus;
	FTimeline m_timeline_camera_execution_front;
	FTimeline m_timeline_camera_execution_back;
#pragma endregion

#pragma region Curves
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_camera_focus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_camera_execution_front;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_camera_execution_back;
#pragma endregion

#pragma region Timer
	FTimerHandle m_timer_hit_react;
	FTimerHandle m_timer_lockon;
#pragma endregion

#pragma region Values
	bool	m_is_sprint;
	bool	m_is_guard;
	bool	m_is_not_damage_mod;
	bool	m_enable_sprint_turn;
	bool	m_enable_attack;
	bool	m_enable_attack_short;
	bool	m_enable_input_movement;
	bool	m_enable_input_attack_rotate;

	float	m_spring_arm_length;

	UPROPERTY(EditDefaultsOnly, Category = "Gold Amount")
	int32	m_total_gold;

	int32	m_total_potion;
	FVector	m_location_jump_start;

	TArray<TTuple<USkeletalMeshComponent*, UItemObject*>> m_equiped_item;
#pragma endregion
};
