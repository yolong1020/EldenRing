// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/TransformNonVectorized.h"
#include "Characters/CharacterTypes.h"
#include "GameCharacter_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameCharacter_Interface : public UInterface
{
	GENERATED_BODY()
};

class ELDENRING_API IGameCharacter_Interface
{
	GENERATED_BODY()

public:
	virtual const bool IsNotDamageMod() const PURE_VIRTUAL(IGameCharacter_Interface::IsNotDamageMod, return false; );
	virtual const bool IsShowingWeapon() const PURE_VIRTUAL(IGameCharacter_Interface::IsShowingWeapon, return false; );
	virtual const bool IsHasTag(const FName& TagName) const PURE_VIRTUAL(IGameCharacter_Interface::IsHasTag, return false; );

	virtual void AddIgnoreTargets(AActor* const Target) PURE_VIRTUAL(IGameCharacter_Interface::AddIgnoreTarget, );
	virtual const TArray<AActor*> GetIgnoreTargets() const PURE_VIRTUAL(IGameCharacter_Interface::GetIgnoreTarget, return TArray<AActor*>(); );
	
	virtual const int32 GetTotalGold() const PURE_VIRTUAL(IGameCharacter_Interface::GetTotalGold, return 0; );
	virtual TSubclassOf<class AGameCharacter> GetClassType() const PURE_VIRTUAL(IGameCharacter_Interface::GetClassType, return nullptr; );
	virtual TArray<TSubclassOf<class AWeapon_Actor>> GetWeaponClassTypes() const PURE_VIRTUAL(IGameCharacter_Interface::GetWeaponClassTypes, return TArray<TSubclassOf<class AWeapon_Actor>>(); );
	
	virtual const uint8 GetWeaponEquipState() const PURE_VIRTUAL(IGameCharacter_Interface::GetWeaponEquipState, return 0; );
	virtual void SetWeaponEquipState(const EEquipState& WeaponEquipState) PURE_VIRTUAL(IGameCharacter_Interface::SetWeaponEquipState, );

	virtual const FTransform GetStartTransform() const PURE_VIRTUAL(IGameCharacter_Interface::GetStartTransform, return FTransform(); );
	virtual void SetStartTransform(const FTransform& Transform) PURE_VIRTUAL(IGameCharacter_Interface::SetStartTransform, );
	virtual void SetCharacterTransform(const FTransform& Transform) PURE_VIRTUAL(IGameCharacter_Interface::SetCharacterTransform, );
};
