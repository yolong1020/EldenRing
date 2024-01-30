// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "Items/Weapons/WeaponTypes.h"
#include "Animation/AnimInstance.h"
#include "FieldObject/AssemblyPoint/AssemblePointObjectTypes.h"
#include "GameCharacter_AnimInstance.generated.h"

class ANPC_Character;
class UCharacterMovementComponent;

UCLASS()
class ELDENRING_API UGameCharacter_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float delta) override;

	void SetHitReactProperties(const bool& is_need_blend, const int32& animation_index, const EAttackWeight& attack_weight, const EHitCycleType& cycle_type);

	FORCEINLINE void  IsNeedBlendPerBone(bool is_need)				{ m_is_blend_per_bone = is_need; }
	FORCEINLINE void  SetHitCycle(const EHitCycleType& hit_cycle)	{ m_hit_cycle_type = hit_cycle; }
	FORCEINLINE const EHitCycleType& GetHitCycle()					{ return m_hit_cycle_type; }

private:
	void RestTransformFix(const float& delta);

public:
	UPROPERTY(BlueprintReadOnly)
	ANPC_Character* m_npc;

	UPROPERTY(BlueprintReadOnly, Category = Death)
	EDeathPose m_death_pose;

	UPROPERTY(BlueprintReadOnly, Category = Battle)
	EBattlePose m_battle_pose;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* m_npc_movement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float m_ground_speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_falling = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_death_by_execution = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_reached_apex = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool m_is_blend_per_bone;

	UPROPERTY(BlueprintReadOnly, Category = "Hit React")
	int32 m_hit_animation_index;

	UPROPERTY(BlueprintReadOnly, Category = "Hit React")
	EAttackWeight m_hit_attack_weight;

	UPROPERTY(BlueprintReadOnly, Category = "Hit React")
	EHitCycleType m_hit_cycle_type;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Action State")
	EActionState_NPC m_action_state;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Equip State")
	EEquipState m_equip_state;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Vigilance State")
	EVigilanceState m_vigilance_state;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Rest State")
	EAssemblyPointObjectType m_rest_state;

public:
#pragma region Notify
	UFUNCTION()
	void AnimNotify_MontageStart();

	UFUNCTION()
	void AnimNotify_MontageEnd();

	UFUNCTION()
	void AnimNotify_EnableBoxCollision();

	UFUNCTION()
	void AnimNotify_DisableBoxCollision();

	UFUNCTION()
	void AnimNotify_OnVigilance();

	UFUNCTION()
	void AnimNotify_OnRepose();

	UFUNCTION()
	void AnimNotify_AfterDeath();

	UFUNCTION()
	void AnimNotify_OnNextAttack();

	UFUNCTION()
	void AnimNotify_OnEndAttack();

	UFUNCTION()
	void AnimNotify_OnReactEnd();

	UFUNCTION()
	void AnimNotify_OnRotateEnd();

	UFUNCTION()
	void AnimNotify_OnEndStunn();

	UFUNCTION()
	void AnimNotify_OnTakeExecutionEnd();

	UFUNCTION()
	void AnimNotify_OnWatchingToIdle();

	UFUNCTION()
	void AnimNotify_OnIdleToWatching();

	UFUNCTION()
	void AnimNotify_OnRestingToIdle();

	UFUNCTION()
	void AnimNotify_OnIdleToResting();

	UFUNCTION()
	void AnimNotify_OnSwapWeaponR();

	UFUNCTION()
	void AnimNotify_OnChangeDeathFront();

	UFUNCTION()
	void AnimNotify_OnChangeDeathBack();
#pragma endregion

};
