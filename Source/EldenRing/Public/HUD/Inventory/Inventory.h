// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

class UInventoryComponent;

UCLASS()
class BASIC_API UInventory : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const float& GetTileSize() { return m_tile_size; }
	
protected:
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Component")
	UInventoryComponent* m_inventory_component;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Constant")
	float m_tile_size;
};
