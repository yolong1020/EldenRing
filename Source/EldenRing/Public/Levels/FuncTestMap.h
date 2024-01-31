// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/BaseLevelScript.h"
#include "FuncTestMap.generated.h"

UCLASS()
class ELDENRING_API AFuncTestMap : public ABaseLevelScript
{
	GENERATED_BODY()
	
public:
	virtual void LoadedLevel() override final {};
	
protected:
	virtual void BeginPlay() override;
};
