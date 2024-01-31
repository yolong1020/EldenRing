// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UITypes.h"
#include "Items/ItemTypes.h"
#include "ItemInfo.generated.h"

class UTextBlock;

UCLASS()
class ELDENRING_API UItemInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateItemInfo(FVector2D Position, const FVector2D& SlotSize, const TObjectPtr<class UItemObject> ItemObject, const bool& IsShowPrice);
	
protected:
	virtual void NativeOnInitialized() override final;

private:
	UFUNCTION()
	void SetRenderPosition();

	void AddOptions();
	void ClearOptions();

protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override final;

private:
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* ItemName;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UTextBlock* ItemDesc;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInvalidationBox> InvalidationBox;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UVerticalBox> OptionList;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UItemPrice> Price;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> L_Desc;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> R_Desc;

	UPROPERTY()
	TWeakObjectPtr<class UDataTableManager> m_data_mgr;

	UPROPERTY()
	UClass*				m_option_class;

	UPROPERTY()
	TObjectPtr<UItemObject> m_item_object;

	FVector2D			m_position;
	FVector2D			m_slot_size;
	mutable FVector2D	m_widget_size;
};
