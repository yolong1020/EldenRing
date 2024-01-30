// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structure/InventoryTile.h"
#include "UI/UIDelegates.h"
#include "InventoryInteractionGrid.generated.h"

class	UBorder;
class	UCanvasPanel;
class	UInventoryItem;
class	UInventorySlotGrid;
class   UDragDropOperation;
class	USlateBrushAsset;
struct	FInventoryLine;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ELDENRING_API UInventoryInteractionGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitInteractionGridWidget(const FMargin& margin);
	void GetMousePositionInTile(const FVector2d& mouse_position, bool& is_right, bool& is_down);

	static const TObjectPtr<UItemObject> GetPayload(UDragDropOperation* operation);

	UFUNCTION()
	void RefreshGridWidget();

protected:
	virtual void	NativeOnInitialized() override final;
	virtual int32   NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool	NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply  NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply	NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<class USizeBox> SizeBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<class UBorder> GridBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	TObjectPtr<class UCanvasPanel> Canvas;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItem> m_widget_class;

	UPROPERTY()
	TWeakObjectPtr<class UStorageManager> m_storage_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UEquipmentManager> m_equipment_mgr;

	UPROPERTY()
	TObjectPtr<USlateBrushAsset> m_brush;

	double		m_tile_size;
	bool		m_draw_drop_location;
	int32		m_colum;
	FVector2D	m_space_size;

	FInventoryTile  m_start_tile_cursor;
	FRefreshInteractionGrid m_refresh_delegate;
};
