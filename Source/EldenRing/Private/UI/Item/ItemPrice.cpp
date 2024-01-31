// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Item/ItemPrice.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/TextBlock.h"

void UItemPrice::InitPriceText(const EItemTradeType& TradeType, const int32& PriceValue)
{
	FText text;
	switch (TradeType)
	{
		case EItemTradeType::EITT_Sell:		text = FText::FromString(TEXT("판매가")); break;
		case EItemTradeType::EITT_Purchase:	text = FText::FromString(TEXT("구매가")); break;
	}

	Option->SetText(text);

	FString string = UKismetTextLibrary::AsCurrency_Integer(PriceValue, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 3).ToString();
	string.RemoveAt(0);

	Value->SetText(FText::FromString(string));
}
