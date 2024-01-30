// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ERGameState.generated.h"

class ATreasure;

UCLASS()
class ELDENRING_API AERGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void SpawnItemFromObject(APawn* Pawn, const TObjectPtr<class UItemObject> ItemObject, const bool& IsGroundClamp);
	void SpawnGoldFromObject(APawn* Pawn, const int32& GoldAmount, const bool& IsGroundClamp);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATreasure> m_gold_class;
};
