// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPC/NPC_Character.h"
#include "C4311.generated.h"

UCLASS()
class BASIC_API AC4311 : public ANPC_Character
{
	GENERATED_BODY()

public:
	AC4311();

	virtual void  Tick(float DeltaTime) override;
	virtual void  GetHit_Implementation(const FVector& ImpactPoint, const EAttackWeight& attack_weight) override final;
	virtual void  SetWeaponCollision(ECollisionEnabled::Type type) override final;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override final;

	virtual void OnChangeVigilanceState(const EVigilanceState& state) override final;
	virtual void OnMoveCompleted(const FPathFollowingResult& Result) override final;
	virtual void OnNextAttack() override final;
	virtual void OnEndAttack() override final;
	virtual void OnTurnEnd() override final;
	virtual void OnAttackDefended(const EAttackWeight& attack_weight) override final;
	virtual void OnAttackBlocked(const EAttackWeight& attack_weight) override final;
	virtual void OnTakeStunn() override final;
	virtual void OnTakeExecutionEnd() override final;
	virtual void OnWatchingSwitch() override final;
	virtual void OnRestingSwitch() override final;
	virtual void OnRestingEnd(const FString& section_name) override final;

	virtual bool InAssmeblyPointAction() override final;

protected:
	virtual void BeginPlay() override;

	virtual void HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight) override final;
	virtual void PawnSeen(APawn* seen_pawn) override;
	
	virtual void StartWatching() override final;
	virtual void StartCombat() override final;
	virtual void StartConfront() override final;
	virtual void StartAttack() override final;
	virtual void StartTurn(const EGameDirection& direction) override final;

	virtual void OnCheckMissingTarget() override final;
	virtual void OnWatchingFinished() override final;
	virtual void OnReactEnd() override final;

private:
	FName	ChooseNextAttack();
	void	ChooseNextAction();
	void	FinishVigilance();

	bool	IsCantMoveState();

private:
#pragma region Montage
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* m_montage_swap;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* m_montage_watching;
#pragma endregion
};
