// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InventoryItemComponent.generated.h"

class UInventoryItem;

UCLASS()
class BASIC_API UInventoryItemComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UInventoryItem* m_widget_item;
};
