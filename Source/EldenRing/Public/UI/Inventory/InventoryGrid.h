// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "../Structure/InventoryTile.h"
#include "InventoryGrid.generated.h"

class UBorder;
class UCanvasPanel;
class UInventoryItem;
class UInventorySlotGrid;
class UInventoryInteractionGrid;
class UDragDropOperation;
class UTextBlock;
class UButton;
class UModalPopUp;

struct FInventoryLine;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ELDENRING_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitGridWidget();

	UFUNCTION()
	void RefreshGoldAmount(const int32& gold_amount);

protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void OnClickedGoldInteraction();

public:
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

	UPROPERTY()
	TWeakObjectPtr<class UUIManager> m_ui_mgr;
};
