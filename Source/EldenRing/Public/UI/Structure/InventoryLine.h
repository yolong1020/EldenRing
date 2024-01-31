// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryLine.generated.h"

USTRUCT(BlueprintType)
struct ELDENRING_API FInventoryLine
{
	GENERATED_USTRUCT_BODY()
	
public:
	explicit FInventoryLine() : Start(FVector2D::ZeroVector), End(FVector2D::ZeroVector) {};
	~FInventoryLine()	{};

	FInventoryLine(const FVector2D& start, const FVector2D& end);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	FVector2D Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	FVector2D End;
};
