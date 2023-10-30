// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structure/InventoryTile.h"
#include "../HUD_Delegates.h"
#include "InventorySlotGrid.generated.h"

class	UInventoryComponent;
class	UBorder;
class	UCanvasPanel;
class	UItemObject;
class	UInventoryItem;
class	UInventorySlot;
class   UDragDropOperation;
struct	FInventoryLine;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASIC_API UInventorySlotGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitGridSlotWidget(UInventoryComponent* inventory_component, const int32& colum_count, const int32& row_count);

	FORCEINLINE const FVector2D& GetBorderSize()		{ return m_border_size; }
	FORCEINLINE const FVector2D& GetBorderPosition()	{ return m_border_position; }

public:
	UPROPERTY(BlueprintReadOnly, Category = "Component")
	UInventoryComponent* m_inventory_component;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	UBorder* GridBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	UCanvasPanel* GridCanvasPanel;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventorySlot> m_widget_class;

private:
	FVector2D	m_border_size;
	FVector2D	m_border_position;
	float		m_tile_size;

	int32		m_colums;
	int32		m_rows;
};
