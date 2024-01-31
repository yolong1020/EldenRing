// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemTypes.h"
#include "ItemPrice.generated.h"

UCLASS()
class ELDENRING_API UItemPrice : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitPriceText(const EItemTradeType& TradeType, const int32& PriceValue);

private:
	UPROPERTY(BlueprintReadWrite, Category = "Option Text", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Option;

	UPROPERTY(EditAnywhere, Category = "Option Text", meta = (editcondition = "bOverride_Text"))
	FText OptionText;

	UPROPERTY(BlueprintReadWrite, Category = "Value Text", meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Value;

	UPROPERTY(EditAnywhere, Category = "Value Text", meta = (editcondition = "bOverride_Text"))
	FText ValueText;
};
