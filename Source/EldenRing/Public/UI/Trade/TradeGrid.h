// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "../Structure/InventoryTile.h"
#include "TradeGrid.generated.h"

class UInventoryItem;
class UInventorySlotGrid;
class UTradeInteractionGrid;
class UTextBlock;

struct FInventoryLine;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ELDENRING_API UTradeGrid : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitGridWidget(const bool IsPlayer);
	
	UFUNCTION()
	void RefreshGoldAmount(const int32& gold_amount);
	void RefreshOwnerName(const FString& Name);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItem> m_widget_class;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	UInventorySlotGrid* m_widget_slot_grid;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	UTradeInteractionGrid* m_widget_Interaction_grid;

	UPROPERTY(EditDefaultsOnly, Category = "Margin Setting", meta = (AllowPrivateAccess = "true"))
	FMargin m_margin;

	UPROPERTY(BlueprintReadOnly, Category = "Title", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* OwnerName;

	UPROPERTY(BlueprintReadOnly, Category = "Gold Setting", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* GoldAmount;

	FInventoryTile m_start_tile_draged;
};
