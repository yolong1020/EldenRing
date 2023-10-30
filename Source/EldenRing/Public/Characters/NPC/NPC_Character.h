// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GameCharacter.h"
#include "NPC_Character.generated.h"

class UHealthBarComponent;
class AGameCharacter_AIController;
class UGameCharacter_AnimInstance;
class AAssemblyPoint;
class AAssemblePointObject;
struct FPathFollowingResult;

UCLASS()
class BASIC_API ANPC_Character : public AGameCharacter
{
	GENERATED_BODY()

public:
	ANPC_Character();

	virtual void	Tick(float DeltaTime) override;
	virtual void	Destroyed() override;

	virtual float	TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void	TakeExecution(AActor* target, const EGameDirection& direction, const int32& damage) override;
	
	virtual bool	IsCurrentActionState(const FString& action) override;
	virtual bool	IsGuardState() override;

	FORCEINLINE const EActionState_NPC&		GetActionState()		{ return m_action_state; }
	FORCEINLINE const EVigilanceState&		GetVigilanceState()		{ return m_vigilance_state; }
	FORCEINLINE const bool&				GetWatchingSwitch()		{ return m_is_idle_to_watching; }
	FORCEINLINE const bool&				GetRestingSwitch()		{ return m_is_idle_to_resting; }
	FORCEINLINE AAssemblyPoint* const		GetAssemblyPoint()		{ return m_assembly_point; }
	FORCEINLINE AAssemblePointObject* const		GetAssemblyPointObject()	{ return m_target_assembly_object; }
	FORCEINLINE const bool&				IsPatrol()			{ return m_is_patrolling; }

	UFUNCTION(BlueprintCallable)
	void SuccessAttack() { m_attack_success = true; }
	void InitAssemblyPointObject(AAssemblePointObject* const point_object);

	virtual void OnParryStart() override {};
	virtual void OnParryEnd() override {};
	virtual void OnReactEnd() override {};
	virtual void OnAttackDefended(const EAttackWeight& attack_weight) override {};
	virtual void OnAttackBlocked(const EAttackWeight& attack_weight) override {};
	virtual void OnChangeVigilanceState(const EVigilanceState& state);
	virtual void OnTurnEnd();
	virtual void OnEndStunn();
	virtual void OnDeathCompleted();
	virtual void OnSwapWeaponR();
	virtual bool IsNeedChangeDeath(const EGameDirection& direction);
	virtual void OnNextAttack() 	= 0;
	virtual void OnEndAttack()  	= 0;
	virtual void OnWatchingSwitch() = 0;
	virtual void OnRestingSwitch() 	= 0;
	virtual void OnRestingEnd(const FString& section_name) = 0;
	virtual void OnMoveCompleted(const FPathFollowingResult& Result) = 0;

	virtual bool InAssmeblyPointAction() { return false; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void PawnSeen(APawn* seen_pawn) {};
	void SetBlindPawn(const bool& is_blind);

	bool EquipWeapon(const FName& socket_name, const EWeaponEquipHand& hand);

	virtual void StartWatching();
	virtual void StartConfront() 	= 0;
	virtual void StartCombat() 	= 0;
	virtual void StartAttack() 	= 0;
	virtual void StartTurn(const EGameDirection& direction) = 0;
	virtual void ClearAllTimer();

	virtual void StopAllMovement();

	virtual void OnCheckMissingTarget() {};
	virtual void OnMoveOtherPatrolPoint();
	virtual void OnWatchingFinished() {};

	virtual void RefreshHealthbar(float DamageAmount = 0) override;

	bool IsInSight(AActor* target);
	bool InTargetRange(AActor* target, double radius);
	void MoveToTarget(AActor* target, const float& accept_radius = -1.f);
	void MoveToLocation(const FVector& dest, const float& acceptance_radius = -1.f, const bool& stop_overlap = true);

	AActor* SelectPatrolTarget();

private:
	UPROPERTY()
	AGameCharacter_AIController* m_controller_ai;

protected:
#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	UAnimMontage* m_montage_hit_medium;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	UAnimMontage* m_montage_hit_medium_direction;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	UAnimMontage* m_montage_hit_heavy_direction;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	UAnimMontage* m_montage_hit_extra_heavy_direction;

	UPROPERTY(EditDefaultsOnly, Category = "Hit Montages")
	UAnimMontage* m_montage_hit_ultra_heavy_directiony;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_confront;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* m_montage_resting;
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
	AActor* m_target_patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	TArray<AActor*> m_targets_patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	float m_time_wait_min;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	float m_time_wait_max;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	float m_time_patrol;

	UPROPERTY(EditInstanceOnly, Category = "AI Patroll")
	float m_sec_kwon_missing_target;

	FVector			m_location_assembly_object;
	AAssemblePointObject*	m_target_assembly_object;

	bool m_is_idle_to_watching;
	bool m_is_idle_to_resting;
	bool m_is_patrolling;
	bool m_is_resting;
#pragma endregion

#pragma region States
	UPROPERTY(EditInstanceOnly, Category = "States")
	EActionState_NPC m_action_state    = EActionState_NPC::EASN_Unoccupied;
	EVigilanceState	 m_vigilance_state = EVigilanceState::EVS_Repose;
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
	UHealthBarComponent* m_widget_healthbar;
#pragma endregion

#pragma region AssemblyPoint
	UPROPERTY(EditInstanceOnly, Category = "Assembly Point Properties")
	AAssemblyPoint* m_assembly_point;
#pragma endregion
};
