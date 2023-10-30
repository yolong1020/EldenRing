// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structure/InventoryTile.h"
#include "../HUD_Delegates.h"
#include "InventoryInteractionGrid.generated.h"

class	UInventoryComponent;
class	UBorder;
class	UCanvasPanel;
class	UItemObject;
class	UInventoryItem;
class	UInventorySlotGrid;
class   UDragDropOperation;
struct	FInventoryLine;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BASIC_API UInventoryInteractionGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitInteractionGridWidget(UInventoryComponent* inventory_component, const int32& colum_count, const int32& row_count, const FMargin& margin);
	void RefreshGridWidget();
	void GetMousePositionInTile(const FVector2d& mouse_position, bool& is_right, bool& is_down);

	static UItemObject* GetPayload(UDragDropOperation* operation);

	FORCEINLINE const FInventoryTile& GetStartTileDraged()	{ return m_start_tile_draged; }

protected:
	virtual int32   NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool	NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply  NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply	NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Component")
	UInventoryComponent* m_inventory_component;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	UBorder* GridBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	UCanvasPanel* GridCanvasPanel;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItem> m_widget_class;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	UInventorySlotGrid* m_widget_slot_grid;

	FItemOnRemoved	m_remove_item_delegate;

	FInventoryTile	m_start_tile_draged;
	double		m_tile_size;
	bool		m_draw_drop_location;
};
