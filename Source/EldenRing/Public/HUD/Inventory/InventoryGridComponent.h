// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InventoryGridComponent.generated.h"

class UInventoryGrid;

UCLASS()
class BASIC_API UInventoryGridComponent : public UWidgetComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UInventoryGrid* m_widget_grid;
};
