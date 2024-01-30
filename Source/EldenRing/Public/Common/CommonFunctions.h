// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"

struct FCommonFunctions
{
public:
	static const EGameDirection FindDirection(const AActor* Origin, const FVector& Target);
};
