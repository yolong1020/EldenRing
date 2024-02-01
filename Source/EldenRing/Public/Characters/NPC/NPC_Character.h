// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GameCharacter.h"
#include "System/Event/EventDelegates.h"
#include "NPC_Interfaces.h"
#include "NPC_Character.generated.h"

class UHealthBarComponent;
class AGameCharacter_AIController;
class UGameCharacter_AnimInstance;
class AAssemblyPoint;
class AAssemblePointObject;
struct FPathFollowingResult;

UCLASS(Abstract, Blueprintable)
class ELDENRING_API ANPC_Character : public AGameCharacter, public INPC_Interfaces
{
	GENERATED_BODY()

public:
	ANPC_Character();

	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	virtual void EquipWeaponToStart() PURE_VIRTUAL(ANPC_Character::EquipWeaponToStart, );
	virtual void SetWeaponClassTypes(const TArray<TSubclassOf<class AWeapon_Actor>>& WeaponTypes) override final;
	virtual void SetPatrolPoints(const TSet<FString>& Points) override final;
	virtual void SetAssemblyPoint(const FString& AssemblyPoint) override final;
	virtual void SetTimeValues(const float& WaitTimeMin, const float& WaitTimeMax, const float& PatrolTime, const float& KnowMissingTarget) override final;
	virtual void RegisteAssemblyPointMember() override final;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void TakeExecution(AActor* target, const EGameDirection& direction, const int32& damage) override;
	
	virtual const bool IsCurrentActionState(const FString& action) const override final;
	virtual const bool IsGuardState() const override final;
	virtual bool IsNeedChangeDeath(const EGameDirection& direction);

	UFUNCTION(BlueprintCallable)
	virtual void OnTurnEnd();
	virtual void OnTurnEndForAssembly();
	virtual void OnChangeVigilanceState(const EVigilanceState& state);
	virtual void OnEndStunn();
	virtual void OnDeathCompleted();
	virtual void OnSwapWeaponR() {};
	virtual void OnNextAttack() 	PURE_VIRTUAL(ANPC_Character::OnNextAttack, );
	virtual void OnEndAttack()  	PURE_VIRTUAL(ANPC_Character::OnEndAttack, );
	virtual void OnWatchingSwitch() PURE_VIRTUAL(ANPC_Character::OnWatchingSwitch, );
	virtual void OnRestingSwitch();
	virtual void OnRestingEnd(const FString& section_name);
	virtual void OnMoveCompleted(const FPathFollowingResult& Result) PURE_VIRTUAL(ANPC_Character::OnMoveCompleted, );

	void InitAssemblyPointObject(AAssemblePointObject* const point_object);
	virtual const bool InAssmeblyPointAction();
	virtual void MoveToAssemblePoint(AAssemblePointObject* const PointObject);
	virtual void MoveToPatrolPoint();

	FORCEINLINE virtual void		SuccessAttack()			override { m_attack_success = true; }
	FORCEINLINE virtual const float		GetTrackingRadius()		override { return m_radius_tracking; }
	FORCEINLINE virtual const float		GetConfrontingRadius()		override { return m_radius_confront; }
	FORCEINLINE virtual const float		GetAttackRadius()		override { return m_radius_attack; }
	FORCEINLINE virtual const uint8		GetActionState()		override { return (uint8)m_action_state; }
	FORCEINLINE virtual const uint8		GetCombatAction()		override { return (uint8)m_action_combat; }
	FORCEINLINE virtual const FVector	GetTargetLocation()		override { return m_actor_target.Get() ? m_actor_target->GetActorLocation() : FVector::ZeroVector; }
	FORCEINLINE virtual const uint8		GetTargetDirectionFromNPC()	override;
	
	FORCEINLINE const EVigilanceState&					GetVigilanceState() const			{ return m_vigilance_state; }
	FORCEINLINE const bool&							GetWatchingSwitch() const			{ return m_is_idle_to_watching; }
	FORCEINLINE const bool&							GetRestingSwitch()  const			{ return m_is_resting; }

	FORCEINLINE virtual const TArray<TObjectPtr<class APatrolPoint>> 	GetPatrolPoints() const override final  	{ return m_targets_patrol; }
	FORCEINLINE AAssemblyPoint*	const					GetAssemblyPoint() const			{ return m_assembly_point; }
	FORCEINLINE virtual const FString					GetAssemblyPointLabel()	const override final;
	FORCEINLINE AAssemblePointObject* const					GetAssemblyPointObject() const			{ return m_target_assembly; }
	FORCEINLINE const bool&							IsPatrol() const				{ return m_is_patrolling; }
	FORCEINLINE const bool							IsInSight(AActor* target);

	FORCEINLINE virtual const float						GetWaitTime_Min() const override final		{ return m_time_wait_min; };
	FORCEINLINE virtual const float						GetWaitTime_Max() const override final		{ return m_time_wait_max; };
	FORCEINLINE virtual const float						GetPatrolTime()	  const override final		{ return m_time_patrol; };
	FORCEINLINE virtual const float						GetKnowMissingTargetTime() const override final	{ return m_sec_kwon_missing_target; };
	
	FORCEINLINE virtual void SetNPCActionState(const EActionState_NPC& State) override final { m_action_state = State; }

private:
	UFUNCTION()
	void StartPatrolTimer();

protected:
	virtual void BeginPlay() override;
	virtual void OnDeath(TWeakObjectPtr<UAnimMontage> Montage, const float& Duration = 0) override;
	virtual void GameStartAction() override;

	UFUNCTION()
	virtual void PawnSeen(APawn* seen_pawn) {};
	void SetBlindPawn(const bool& is_blind);

	bool EquipWeapon(const FName& socket_name, const EWeaponEquipHand& hand);

	virtual void StartWatching();
	virtual void StartConfront() PURE_VIRTUAL(ANPC_Character::StartConfront, );
	virtual void StartCombat()   PURE_VIRTUAL(ANPC_Character::StartCombat, );
	virtual void StartAttack()   PURE_VIRTUAL(ANPC_Character::StartAttack, );
	virtual void StartTurn(const EGameDirection& direction) PURE_VIRTUAL(ANPC_Character::StartTurn, );
	virtual void StartPatrol() override final;
	virtual void ClearAllTimer();

	virtual void StopAllMovement();
	virtual void OnCheckMissingTarget() PURE_VIRTUAL(ANPC_Character::OnCheckMissingTarget, );
	virtual void OnWatchingFinished()   PURE_VIRTUAL(ANPC_Character::OnWatchingFinished, );

	virtual void RefreshHealthbar(float DamageAmount = 0) override;

	UFUNCTION()
	virtual void SaveCharacterData();

	virtual void AfterTargetDeath(TObjectPtr<class AGameCharacter> Target);

	virtual const bool IsCantMoveState() PURE_VIRTUAL(ANPC_Character::IsCantMoveState, return false; );
	const bool IsInTargetRange(AActor* target, double radius);

	void MoveToTarget(AActor* target, const float& accept_radius = -1.f);
	void MoveToLocation(const FVector& dest, const float& acceptance_radius = -1.f, const bool& stop_overlap = true);

	TObjectPtr<APatrolPoint> SelectPatrolTarget();

protected:
	UPROPERTY()
	class AAIController* m_controller_ai = nullptr;

#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	TObjectPtr<UAnimMontage> m_montage_hit_medium;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	TObjectPtr<UAnimMontage> m_montage_hit_medium_direction;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	TObjectPtr<UAnimMontage> m_montage_hit_heavy_direction;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	TObjectPtr<UAnimMontage> m_montage_hit_extra_heavy_direction;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	TObjectPtr<UAnimMontage> m_montage_hit_ultra_heavy_directiony;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_confront;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> m_montage_resting;
#pragma endregion

#pragma region Navigation
	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	double m_radius_patrol;

	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	double m_radius_attack;

	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	double m_radius_confront;

	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	double m_radius_tracking;

#pragma endregion

#pragma region Patrol
	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	TObjectPtr<class APatrolPoint> m_target_patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	TArray<TObjectPtr<class APatrolPoint>> m_targets_patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	float m_time_wait_min;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	float m_time_wait_max;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	float m_time_patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	float m_sec_kwon_missing_target;

	UPROPERTY()
	TObjectPtr<AAssemblePointObject> m_target_assembly;

	FRequestAllowUsing m_request_callback;

	bool m_is_idle_to_watching;
	bool m_is_resting;
	bool m_is_patrolling;
#pragma endregion

#pragma region Combat
	UPROPERTY()
	TArray<AGameCharacter*> m_combat_list;
#pragma endregion

#pragma region States
	UPROPERTY(EditInstanceOnly, Category = "States")
	EActionState_NPC m_action_state = EActionState_NPC::EASN_Unoccupied;

	UPROPERTY(EditInstanceOnly, Category = "States")
	ECombatAction_NPC m_action_combat;

	EVigilanceState	m_vigilance_state = EVigilanceState::EVS_Repose;
#pragma endregion

#pragma region Timer
	FTimerHandle m_timer_sight;
	FTimerHandle m_timer_watching;
	FTimerHandle m_timer_patrol;
	FTimerHandle m_timer_reserve_action;
	FTimerHandle m_timer_hit;
#pragma endregion

#pragma region HUD
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> m_widget_healthbar = nullptr;
#pragma endregion

#pragma region AssemblyPoint
	UPROPERTY(EditInstanceOnly, Category = "Assembly Point Properties")
	TObjectPtr<AAssemblyPoint> m_assembly_point;
#pragma endregion

#pragma region Weapons
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeapon_Actor> m_class_weapon_R;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeapon_Actor> m_class_weapon_L;
#pragma endregion
};
