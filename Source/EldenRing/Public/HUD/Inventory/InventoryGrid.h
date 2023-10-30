// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structure/InventoryTile.h"
#include "../HUD_Delegates.h"
#include "InventoryGrid.generated.h"

class	UInventoryComponent;
class	UBorder;
class	UCanvasPanel;
class	UItemObject;
class	UInventoryItem;
class	UInventorySlotGrid;
class	UInventoryInteractionGrid;
class   UDragDropOperation;
class   UTextBlock;
class   UButton;
class   UModalPopUp;
struct	FInventoryLine;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASIC_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitGridWidget(UInventoryComponent* inventory_component, const float& tile_size, const int32& colum_count, const int32& row_count);
	void RefreshGridWidget();
	void RefreshGoldAmount(const int32& gold_amount);
	
	const FInventoryTile& GetStartTileDraged();

private:
	UFUNCTION()
	void OnClickedGoldInteraction();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Component")
	UInventoryComponent* m_inventory_component;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItem> m_widget_class;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UModalPopUp> m_modal_class;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	UInventorySlotGrid* m_widget_slot_grid;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	UInventoryInteractionGrid* m_widget_Interaction_grid;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* GoldAmount;

	UPROPERTY(BlueprintReadOnly, Category = "UI Widget", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UButton* ButtonGoldInteraction;

	UPROPERTY(EditDefaultsOnly, Category = "Margin", meta = (AllowPrivateAccess = "true"))
	FMargin m_margin;

	FItemOnRemoved			m_remove_item_delegate;
	FInventoryTile			m_start_tile_draged;

	int32					m_colum_count;
	int32					m_row_count;
	float					m_tile_size;

	bool					m_draw_drop_location;
};
