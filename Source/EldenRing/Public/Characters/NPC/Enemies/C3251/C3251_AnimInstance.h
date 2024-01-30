// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/NPC/Enemies/C3251/C3251.h"
#include "Characters/NPC/NPC_Interfaces.h"
#include "C3251_AnimInstance.generated.h"

UCLASS()
class ELDENRING_API UC3251_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float delta) override;

public:
#pragma region Notify
	UFUNCTION()
	void AnimNotify_OnRootMotion();

	UFUNCTION()
	void AnimNotify_OnJump();

	UFUNCTION()
	void AnimNotify_OnEndJump();

	UFUNCTION()
	void AnimNotify_OnLockRotation();

	UFUNCTION()
	void AnimNotify_OnUnlockRotation();

	UFUNCTION()
	void AnimNotify_OnShowDirecting();

	UFUNCTION()
	void AnimNotify_OnBattle();

	UFUNCTION()
	void AnimNotify_OnEndAttack();

	UFUNCTION()
	void AnimNotify_OnEndMove();

	UFUNCTION()
	void AnimNotify_OnInputCombo();

	UFUNCTION()
	void AnimNotify_OnRotate();

	UFUNCTION()
	void AnimNotify_OnEndRotate();

	UFUNCTION()
	void AnimNotify_OnRotateMoving();

	UFUNCTION()
	void AnimNotify_EnableWeaponCollision();

	UFUNCTION()
	void AnimNotify_DisableWeaponCollision();

	UFUNCTION()
	void AnimNotify_EnableBodyCollision();

	UFUNCTION()
	void AnimNotify_DisableBodyCollision();

	UFUNCTION()
	void AnimNotify_EnableLegsCollision();

	UFUNCTION()
	void AnimNotify_DisableLegsCollision();

	UFUNCTION()
	void AnimNotify_EnableShieldCollision();

	UFUNCTION()
	void AnimNotify_DisableShieldCollision();

	UFUNCTION()
	void AnimNotify_EnableBlendPerBone();

	UFUNCTION()
	void AnimNotify_DisableBlendPerBone();
#pragma endregion

private:
	UPROPERTY()
	class AC3251* m_character;

	UPROPERTY()
	class AC3251_Controller* m_controller;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Action State", meta = (AllowPrivateAccess = "true"))
	EActionState_NPC m_action_state;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Rotation", meta = (AllowPrivateAccess = "true"))
	bool m_need_rotate;

	UPROPERTY(BlueprintReadOnly, Category = "Blend Animation by Bone", meta = (AllowPrivateAccess = "true"))
	bool m_is_blend_by_bone = false;
};
