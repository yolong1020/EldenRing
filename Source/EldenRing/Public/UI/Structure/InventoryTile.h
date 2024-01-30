// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryTile.generated.h"

USTRUCT(BlueprintType)
struct ELDENRING_API FInventoryTile
{
public:
	GENERATED_USTRUCT_BODY()

	explicit FInventoryTile(const FInventoryTile& Tile) { X = Tile.X; Y = Tile.Y; };
	explicit FInventoryTile(int32 x, int32 y)			{ X = x; Y = y; };
	explicit FInventoryTile() : X(0), Y(0) {};
	~FInventoryTile() {};

	bool operator==(const FInventoryTile& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	int32 X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	int32 Y;
};
