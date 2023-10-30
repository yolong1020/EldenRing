// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseGameState.generated.h"

class UItemObject;
class ATreasure;

UCLASS()
class BASIC_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void SpawnItemFromActor(AActor* actor, UItemObject* item_object, const bool& is_ground_clamp);
	void SpawnGoldFromActor(AActor* actor, const int32& gold_amount, const bool& is_ground_clamp);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATreasure> m_gold_class;
};
