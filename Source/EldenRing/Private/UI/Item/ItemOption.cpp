// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Item/ItemOption.h"
#include "Components/TextBlock.h"

void UItemOption::InitOptionText(const EItemOptionType& OptionType, const FText& OptionValue)
{
	FText text;
	switch (OptionType)
	{
		case EItemOptionType::EIOT_Attack:	text = FText::FromString(TEXT("공격력"));	break;
		case EItemOptionType::EIOT_Shield:	text = FText::FromString(TEXT("방어력"));	break;
		case EItemOptionType::EIOT_Hp:		text = FText::FromString(TEXT("체력"));		break;
		case EItemOptionType::EIOT_Stamina: text = FText::FromString(TEXT("스테미나"));	break;
		case EItemOptionType::EIOT_Power:	text = FText::FromString(TEXT("힘"));		break;
	}
	Option->SetText(text);
	Value->SetText(OptionValue);
}
