// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryTile.generated.h"

USTRUCT(BlueprintType)
struct BASIC_API FInventoryTile
{
public:
	GENERATED_USTRUCT_BODY()

	FInventoryTile(int32 x, int32 y) { X = x; Y = y; };
	FInventoryTile() {};
	~FInventoryTile() {};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	int32 X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	int32 Y;
};
