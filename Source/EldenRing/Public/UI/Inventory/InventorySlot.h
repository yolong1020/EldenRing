// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UBorder;
class USizeBox;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ELDENRING_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitSlotWidget(const float& slot_size);

private:
	UPROPERTY(BlueprintReadOnly, Category = " UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USizeBox> SizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBorder> SlotBorder;
};
