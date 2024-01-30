// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIDelegates.h"
#include "InventoryItem.generated.h"

class	USizeBox;
class	UBorder;
class	UImage;
class	UItemObject;
struct	FSlateBrush;
struct  FInventoryTile;

UCLASS()
class ELDENRING_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitItemWidget(const TObjectPtr<class UItemObject>  ItemObject, const FVector2D& SpaceSize, const float& TileSize);
	void InitItemWidget(const TObjectPtr<class UItemObject>  ItemObject, const FVector2D& SpaceSize, const float& TileSize, const FVector2D& CustomSize);
	void ItemRefresh(const FVector2D& SpaceSize);

protected:
	virtual void	NativeOnInitialized() override final;
	virtual int32	NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override final;
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override final;
	virtual void	NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override final;
	virtual void	NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	virtual void	NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override final;
	virtual FReply  NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override final;

private:
	void Refresh();

private:
	UPROPERTY()
	TWeakObjectPtr<class UStorageManager> m_storage_mgr;

	UPROPERTY()
	TWeakObjectPtr<class UEquipmentManager> m_equip_mgr;

	UPROPERTY(BlueprintReadOnly, Category = " UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> BGSizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBorder> BGBorder;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY()
	TWeakObjectPtr<UDragDropOperation>	m_widget_drag;

	UPROPERTY()
	TObjectPtr<UItemObject>				m_item_object;

	mutable FVector2D					m_widget_size;
	float								m_tile_size;
	FVector2D							m_slot_size;
	TSubclassOf<UDragDropOperation>		m_drag_drop_class;

};
