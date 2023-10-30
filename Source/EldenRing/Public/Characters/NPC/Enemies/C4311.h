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
	virtual void  SetWeaponCollision(ECollisionEnabled::Type type) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnChangeVigilanceState(const EVigilanceState& state);
	virtual void OnMoveCompleted(const FPathFollowingResult& Result) override;
	virtual void OnNextAttack() override;
	virtual void OnEndAttack() override;
	virtual void OnTurnEnd() override;
	virtual void OnAttackDefended(const EAttackWeight& attack_weight) override;
	virtual void OnAttackBlocked(const EAttackWeight& attack_weight) override;
	virtual void OnTakeStunn();
	virtual void OnTakeExecutionEnd() override;
	virtual void OnWatchingSwitch() override;
	virtual void OnRestingSwitch() override;
	virtual void OnRestingEnd(const FString& section_name) override;

	virtual bool InAssmeblyPointAction() override;

protected:
	virtual void BeginPlay() override;

	virtual void HitReact(const EGameDirection& hit_direction, const EAttackWeight& attack_weight) override final;
	virtual void PawnSeen(APawn* seen_pawn) override;
	
	virtual void StartWatching() override;
	virtual void StartCombat() override;
	virtual void StartConfront() override;
	virtual void StartAttack() override;
	virtual void StartTurn(const EGameDirection& direction) override;

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
