// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/NPC_Character.h"
#include "C4311.generated.h"

UCLASS()
class ELDENRING_API AC4311 : public ANPC_Character
{
	GENERATED_BODY()

public:
	explicit AC4311();
	virtual void Destroyed() override final;

	virtual void  GetHit(const EGameDirection& Dir, const FVector& ImpactPoint, const EAttackWeight& AttackWeight) override final;
	virtual void  SetWeaponCollision(ECollisionEnabled::Type type) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void  EquipWeaponToStart() override final;
	virtual void OnSwapWeaponR() override final;
	virtual void OnChangeVigilanceState(const EVigilanceState& state);
	virtual void OnMoveCompleted(const FPathFollowingResult& Result) override final;
	virtual void OnNextAttack() override final;
	virtual void OnEndAttack() override final;
	virtual void OnAttackDefended(const EAttackWeight& attack_weight) override final;
	virtual void OnAttackBlocked(const EAttackWeight& attack_weight) override final;
	virtual void OnTakeStunn();
	virtual void OnTakeExecutionEnd() override final;
	virtual void OnWatchingSwitch() override final;
	virtual void OnTurnEnd() override final;

	FORCEINLINE virtual const bool IsAttacking() const override final { return m_action_state == EActionState_NPC::EASN_Attacking; }

protected:
	virtual void BeginPlay() override final;
	virtual void OnDeath(TWeakObjectPtr<UAnimMontage> Montage, const float& Duration = 0) override final;

	virtual void HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight) override final;
	virtual void PawnSeen(APawn* seen_pawn) override;

	virtual void FinishFadeOut(class UNiagaraComponent* PSystem) override final;

	virtual void StartWatching() override;
	virtual void StartCombat() override;
	virtual void StartConfront() override;
	virtual void StartAttack() override;
	virtual void StartTurn(const EGameDirection& direction) override;

	virtual void OnCheckMissingTarget() override final;
	virtual void OnWatchingFinished() override final;
	virtual void OnReactEnd() override final;

	virtual const bool IsCantMoveState() override final;
	virtual void AfterTargetDeath(TObjectPtr<class AGameCharacter> Target) override final;
	virtual void GameStartAction() override final;

private:
	FName	ChooseNextAttack();
	void	ChooseNextAction();

	UFUNCTION()
	void	FinishVigilance();

private:
#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* m_montage_swap;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* m_montage_watching;
#pragma endregion

#pragma region SubSystem
	UPROPERTY()
	TWeakObjectPtr<class UObserverManager> m_observer_mgr;
#pragma endregion
};
