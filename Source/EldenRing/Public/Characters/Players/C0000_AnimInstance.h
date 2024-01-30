// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "Animation/AnimInstance.h"
#include "C0000_AnimInstance.generated.h"

class AC0000;
class UCharacterMovementComponent;

UCLASS()
class ELDENRING_API UC0000_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Delta) override;

	FORCEINLINE const bool& IsEnableSprintTurn() { return m_enable_sprint_turn; }
	FORCEINLINE void SetBlendPerBone(const bool& is_blend) { m_is_blend_by_bone = is_blend; }
	FORCEINLINE void SetRequestMontage(UAnimMontage* const RequestMontage) { m_request_montage = RequestMontage; }
	void ResetEnableSprintTurn();

private:
	void LockOnTarget(const FVector& last_input_vector, AActor* const actor_target, const float& delta);

	void CheckNotInput(const FVector& last_input_vector);
	void CheckEnableSprintTurn();

	void SetRotateAttackByInput(const float& Delta);
	void SetRotateAttack(const float& Delta);

	bool IsTryingMove();

public:
#pragma region Notify
	UFUNCTION()
	void AnimNotify_MontageStart();

	UFUNCTION()
	void AnimNotify_MontageEnd();

	UFUNCTION()
	void AnimNotify_MontageJumpSection();

	UFUNCTION()
	void AnimNotify_EnableBoxCollision();

	UFUNCTION()
	void AnimNotify_DisableBoxCollision();

	UFUNCTION()
	void AnimNotify_JumpStart();

	UFUNCTION()
	void AnimNotify_OnLandEnd();

	UFUNCTION()
	void AnimNotify_OnDodgeEnd();

	UFUNCTION()
	void AnimNotify_OnTurnEnd();

	UFUNCTION()
	void AnimNotify_OnApex();

	UFUNCTION()
	void AnimNotify_Swap();

	UFUNCTION()
	void AnimNotify_SwapEnd();

	UFUNCTION()
	void AnimNotify_ReserveNextAttack();

	UFUNCTION()
	void AnimNotify_OnAttackEnd();

	UFUNCTION()
	void AnimNotify_OnEnableInput();

	UFUNCTION()
	void AnimNotify_OnDisableInput();

	UFUNCTION()
	void AnimNotify_OnMoveStopStart();

	UFUNCTION()
	void AnimNotify_OnMoveStopEnd();

	UFUNCTION()
	void AnimNotify_OnGuardStart();

	UFUNCTION()
	void AnimNotify_OnGuardEnd();

	UFUNCTION()
	void AnimNotify_OnKnockbackStart();

	UFUNCTION()
	void AnimNotify_OnKnockbackEnd();

	UFUNCTION()
	void AnimNotify_OnEnableParryCollision();

	UFUNCTION()
	void AnimNotify_OnDisableParryCollision();

	UFUNCTION()
	void AnimNotify_OnParryStart();

	UFUNCTION()
	void AnimNotify_OnParryEnd();

	UFUNCTION()
	void AnimNotify_OnCameraShakeIn();

	UFUNCTION()
	void AnimNotify_OnCameraShakeOut();

	UFUNCTION()
	void AnimNotify_OnEnableShortAttack();

	UFUNCTION()
	void AnimNotify_OnDisableShortAttack();

	UFUNCTION()
	void AnimNotify_OnSuccessConsume();

	UFUNCTION()
	void AnimNotify_OnEndConsume();

	UFUNCTION()
	void AnimNotify_OnReactEnd();

	UFUNCTION()
	void AnimNotify_OnUseStamina();

	UFUNCTION()
	void AnimNotify_OnUseStaminaShort();

	UFUNCTION()
	void AnimNotify_OnHideWeapon();

	UFUNCTION()
	void AnimNotify_OnShowWeapon();

	UFUNCTION()
	void AnimNotify_EnableDamageMod();

	UFUNCTION()
	void AnimNotify_DisableDamageMod();

	UFUNCTION()
	void AnimNotify_OnEndExecution();

	UFUNCTION()
	void AnimNotify_OnEndEnterToBoss();
#pragma endregion

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AC0000> m_character;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TObjectPtr<UCharacterMovementComponent> m_movement_component;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float m_ground_speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	FRotator m_lockon_rotation;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_falling = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_only_move_side = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_jumping_move = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_stop_move = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_front_or_right = false;

	UPROPERTY(BlueprintReadOnly, Category = Action)
	bool m_is_guard = false;

	UPROPERTY(BlueprintReadOnly, Category = Action)
	bool m_is_blend_by_bone = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Equip State")
	EEquipState m_equip_state = EEquipState::EES_EquippedOneHandedWeapon;

private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> m_request_montage;

	FRotator m_spine_rotation		= FRotator::ZeroRotator;
			 						
	float	 m_time_delta			= 0;
	float	 m_time_cur_input		= 0;
	float	 m_time_cur_sprint_turn	= 0;
			 
	bool	 m_enable_sprint_turn	= false;
};
