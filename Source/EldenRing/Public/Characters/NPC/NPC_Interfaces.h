// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/CharacterTypes.h"
#include "NPC_Interfaces.generated.h"

DECLARE_DELEGATE(FDoAfterEndAnimation)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNPC_Interfaces : public UInterface
{
	GENERATED_BODY()
};

class ELDENRING_API INPC_Interfaces
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StartCombat() PURE_VIRTUAL(INPC_Interfaces::StartCombat, );
	virtual void StartPatrol() PURE_VIRTUAL(INPC_Interfaces::StartPatrol, );
	virtual void TrackingTarget() PURE_VIRTUAL(INPC_Interfaces::TrackingTarget, );
	virtual void SetCallbackEndAnimation(const FDoAfterEndAnimation& Callback) PURE_VIRTUAL(INPC_Interfaces::DoAfterEndAnimation, );
	virtual void SetWeaponClassTypes(const TArray<TSubclassOf<class AWeapon_Actor>>& WeaponTypes) PURE_VIRTUAL(INPC_Interfaces::SetWeaponTypes, );
	virtual void SetNPCActionState(const EActionState_NPC& State) PURE_VIRTUAL(INPC_Interfaces::SetNPCActionState, );
	virtual void SetPatrolPoints(const TSet<FString>& Points) PURE_VIRTUAL(INPC_Interfaces::SetPatrolPoints, );
	virtual void SetAssemblyPoint(const FString& AssemblyPoint) PURE_VIRTUAL(INPC_Interfaces::SetAssemblyPoint, );
	virtual void SetTimeValues(const float& WaitTimeMin, const float& WaitTimeMax, const float& PatrolTime, const float& KnowMissingTarget) PURE_VIRTUAL(INPC_Interfaces::SetTimeValues, );
	virtual void RegisteAssemblyPointMember() PURE_VIRTUAL(INPC_Interfaces::RegisteAssemblyPointMember, );

	virtual void SuccessAttack() PURE_VIRTUAL(INPC_Interfaces::SuccessAttack, );

	virtual void TryDashAttack() PURE_VIRTUAL(INPC_Interfaces::TryDashAttack, );
	virtual void TrySingleAttack() PURE_VIRTUAL(INPC_Interfaces::TrySingleAttack, );
	virtual void TryComboAttack() PURE_VIRTUAL(INPC_Interfaces::TryComboAttack, );
	virtual void TryMoveAction() PURE_VIRTUAL(INPC_Interfaces::TryMoveAction, );

	virtual void GetOffsetDegreeAndInterptSpeed(float& Degree, float& Speed) PURE_VIRTUAL(INPC_Interfaces::GetOffsetDegreeAndInterptSpeed, );
	virtual const bool HasDerivedAttack() PURE_VIRTUAL(INPC_Interfaces::HasDerivedAttack, return false; );
	virtual const uint8 GetActionState() PURE_VIRTUAL(INPC_Interfaces::GetActionState, return 0; );
	virtual const uint8 GetCombatAction() PURE_VIRTUAL(INPC_Interfaces::GetCombatAction, return 0; );
	virtual const uint8 GetTargetDirectionFromNPC() PURE_VIRTUAL(INPC_Interfaces::GetTargetDirectionFromNPC, return 0; );
	virtual const FVector GetTargetLocation() PURE_VIRTUAL(INPC_Interfaces::GetTargetLocation, return FVector::ZeroVector; );
	virtual const FName GetCurrentActionID() PURE_VIRTUAL(INPC_Interfaces::GetCurrentActionID, return FName(""); );
	virtual const TArray<TObjectPtr<class APatrolPoint>> GetPatrolPoints() const PURE_VIRTUAL(INPC_Interfaces::GetPatrolPoints, return TArray<TObjectPtr<class APatrolPoint>>(); );
	virtual const FString GetAssemblyPointLabel() const PURE_VIRTUAL(INPC_Interfaces::GetAssemblyPointLabel, return FString(); );

	virtual const float GetWaitTime_Min() const PURE_VIRTUAL(INPC_Interfaces::GetWaitTime_Min, return 0; );
	virtual const float GetWaitTime_Max() const PURE_VIRTUAL(INPC_Interfaces::GetWaitTime_Max, return 0; );
	virtual const float GetPatrolTime() const PURE_VIRTUAL(INPC_Interfaces::GetPatrolTime, return 0; );
	virtual const float GetKnowMissingTargetTime() const PURE_VIRTUAL(INPC_Interfaces::GetKnowMissingTargetTime, return 0; );

	virtual const float GetTrackingRadius() PURE_VIRTUAL(INPC_Interfaces::GetTrackingRadius, return 0.f; );
	virtual const float GetConfrontingRadius() PURE_VIRTUAL(INPC_Interfaces::GetConfrontingRadius, return 0.f; );
	virtual const float GetAttackRadius() PURE_VIRTUAL(INPC_Interfaces::GetAttackRadius, return 0.f; );
};
