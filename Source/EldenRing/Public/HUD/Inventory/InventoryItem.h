// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItem.generated.h"

class	USizeBox;
class	UBorder;
class	UImage;
class	UItemObject;
struct	FSlateBrush;
struct  FInventoryTile;

UCLASS()
class BASIC_API UInventoryItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitItemWidget(FItemOnRemoved& func, UItemObject* item_object, const float& tile_size);
	void InitItemWidget(FItemOnRemoved& func, UItemObject* item_object, const float& tile_size, const FVector2D& custom_size);
	void ItemRefresh();
	void EquipedItemRefresh();

protected:
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void	NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void	NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	void Refresh();

private:
	FItemOnRemoved ItemOnRemoved;

	UPROPERTY(BlueprintReadOnly, Category = " UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	USizeBox* BGSizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UBorder* BGBorder;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* ItemImage;
	
	float				m_tile_size;
	FVector2D			m_slot_size;
	UItemObject*			m_item_object;

	TSubclassOf<UDragDropOperation> m_drag_drop_class;
	UDragDropOperation*		m_widget_drag;
};
