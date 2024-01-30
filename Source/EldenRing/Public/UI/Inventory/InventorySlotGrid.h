// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structure/InventoryTile.h"
#include "UI/UIDelegates.h"
#include "InventorySlotGrid.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ELDENRING_API UInventorySlotGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitGridSlotWidget();

	FORCEINLINE const FVector2D& GetBorderSize()		{ return m_border_size; }
	FORCEINLINE const FVector2D& GetBorderPosition()	{ return m_border_position; }

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<class UBorder> GridBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<class UUniformGridPanel> UniformGrid;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UInventorySlot> m_widget_class;

private:
	FVector2D	m_border_size;
	FVector2D	m_border_position;
	float		m_tile_size;

	FIntVector2 m_size; // Row | Col
	int32		m_colums;
	int32		m_rows;
};
