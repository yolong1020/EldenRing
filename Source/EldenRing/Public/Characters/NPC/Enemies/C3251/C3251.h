// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/NPC_Character.h"
#include "Characters/NPC/NPC_CombatActionData.h"
#include "C3251.generated.h"

class AMeleeAttackBody_Actor;

UCLASS()
class ELDENRING_API AC3251 : public ANPC_Character
{
	GENERATED_BODY()
	
public:
	AC3251();
	virtual void Destroyed() override final;

	virtual void  GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight) override final;
	virtual void  SetWeaponCollision(ECollisionEnabled::Type type) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void  EquipWeaponToStart() override final;
	FORCEINLINE virtual const bool IsAttacking() const override final { return m_action_combat != ECombatAction_NPC::ECAN_MoveAction && m_action_combat != ECombatAction_NPC::ECAN_End; }

	virtual void TrackingTarget() override final;
	virtual void OnMoveCompleted(const FPathFollowingResult& Result) override final;

	UFUNCTION()
	void OnEndAttack();
	void OnEndMove();
	void OnBattle();
	void OnInputCombo();
	void SetBodyCollision(const ECollisionEnabled::Type& Type);
	void SetLegsCollision(const ECollisionEnabled::Type& Type);
	void SetShieldCollision(const ECollisionEnabled::Type Type);
	void OnShowDirecting();
	void StartRotateMoving();

	FORCEINLINE virtual void SetCallbackEndAnimation(const FDoAfterEndAnimation& callback) override { m_animation_callback = callback; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight) override final;
	virtual void PawnSeen(APawn* seen_pawn) override;

	virtual void OnMontageStarted(UAnimMontage* Montage) override final;
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override final;

	virtual void StartCombat() override;
	virtual void TryDashAttack() override final;
	virtual void TrySingleAttack() override final;
	virtual void TryComboAttack() override final;
	virtual void TryMoveAction() override final;

	virtual const bool HasDerivedAttack() override final;
	virtual void GetOffsetDegreeAndInterptSpeed(float& Degree, float& Speed) override final;
	FORCEINLINE virtual const FName GetCurrentActionID() override final { return prv_attack_section; }

	virtual void FinishFadeOut(class UNiagaraComponent* PSystem) override;
	virtual void AfterTargetDeath(TObjectPtr<class AGameCharacter> Target) override final;
	virtual void GameStartAction() override final;

private:
	void CancelAction();

	UFUNCTION()
	void IdleMode();

	UFUNCTION()
	void OnRotateMoving(float Delta);

private:
	UPROPERTY()
	TWeakObjectPtr<class AC3251_Controller> m_controller;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBossHealthBarComponent> m_widget_bossbar;

	UPROPERTY()
	TObjectPtr<UDataTable> m_actions;

	UPROPERTY(EditDefaultsOnly, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> m_horse_capsule;

	UPROPERTY(EditDefaultsOnly, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AMeleeAttackBody_Actor> m_attack_leg_L;
	
	UPROPERTY(EditDefaultsOnly, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AMeleeAttackBody_Actor> m_attack_leg_R;
	
	UPROPERTY(EditDefaultsOnly, Category = "Capsule", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AMeleeAttackBody_Actor> m_attack_body;

	UPROPERTY()
	TWeakObjectPtr<class USoundscapeManager> m_sound_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UObserverManager> m_observer_mgr;

	TArray<FNPC_CombatActionData*>	m_combo_singles[4];

	FLatentActionInfo		m_latent;
	FDoAfterEndAnimation	m_animation_callback;
	FName					prv_attack_section;
	bool					m_is_second_phase;
	bool					m_is_canceled;

#pragma region TimeLine
	FTimeline m_tl_rotate;
#pragma endregion

#pragma region Curve
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> m_curve_rotate;
#pragma endregion

#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_on_battle;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_dash_attack;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_dash_attack_phase2;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_single_front;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_single_front_phase2;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_single_left;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_single_left_phase2;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_single_right;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_combo_right;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_combo_back;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_move_action;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_hit_react;
#pragma endregion

	TSet<FString> m_playing_montages;
};
