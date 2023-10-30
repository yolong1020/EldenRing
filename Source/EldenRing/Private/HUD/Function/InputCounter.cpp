// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Function/InputCounter.h"
#include "Characters/Players/C0000.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Kismet/KismetTextLibrary.h"
#include "Basic/DebugMacros.h"

void UInputCounter::InitInputCounter()
{
	AC0000* player = Cast<AC0000>(GetOwningPlayerPawn());
	CHECK_INVALID(player)
	m_owner = player;

	CHECK_INVALID(ButtonUpper)
	CHECK_INVALID(ButtonUnder)	
	CHECK_INVALID(InputText)

	int32 gold = m_owner->GetTotalGold();
	if (gold == 0)
	{
		ButtonUpper->SetIsEnabled(false);
		ButtonUnder->SetIsEnabled(false);
	}

	ButtonUpper->OnClicked.AddDynamic(this, &UInputCounter::OnClickUpper);
	ButtonUnder->OnClicked.AddDynamic(this, &UInputCounter::OnClickUnder);

	InputText->OnTextCommitted.AddDynamic(this, &UInputCounter::OnChnageTextCommited);
}

void UInputCounter::OnClickUpper()
{
	CHECK_INVALID(m_owner)
	CHECK_INVALID(InputText)
	CHECK_INVALID(ButtonUpper)
	CHECK_INVALID(ButtonUnder)

	int32 total_gold  = m_owner->GetTotalGold();
	int32 input_value = FCString::Atoi(*InputText->GetText().ToString());
	++input_value;

	SetTextAsCurrency(input_value);

	if (input_value == total_gold) { ButtonUpper->SetIsEnabled(false); }
	if (false == ButtonUnder->GetIsEnabled()) { ButtonUnder->SetIsEnabled(true); }
}

void UInputCounter::OnClickUnder()
{
	CHECK_INVALID(m_owner)
	CHECK_INVALID(InputText)
	CHECK_INVALID(ButtonUpper)

	int32 total_gold  = m_owner->GetTotalGold();
	int32 input_value = FCString::Atoi(*InputText->GetText().ToString());
	--input_value;

	if (0 > input_value)
	{
		SetTextAsCurrency(total_gold);
		if (ButtonUpper->GetIsEnabled()) { ButtonUpper->SetIsEnabled(false); }
	}
	else
	{
		SetTextAsCurrency(input_value);
		if (false == ButtonUpper->GetIsEnabled()) { ButtonUpper->SetIsEnabled(true); }
	}

}

void UInputCounter::OnChnageTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	CHECK_INVALID(m_owner)
	CHECK_INVALID(InputText)

	FString InputString = Text.ToString();
	bool	bIsNumeric  = InputString.IsNumeric();

	int32 total_gold  = m_owner->GetTotalGold();
	int32 input_value = FCString::Atoi(*InputText->GetText().ToString());

	if (false == bIsNumeric || 0 > input_value)
	{
		InputText->SetText(FText::FromString("0"));
	}
	else if (total_gold < input_value)
	{
		SetTextAsCurrency(total_gold);
	}
}

void UInputCounter::SetTextAsCurrency(const int32& amount)
{
	FString string = UKismetTextLibrary::AsCurrency_Integer(amount, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 3).ToString();
	string.RemoveAt(0);

	InputText->SetText(FText::FromString(string));
}

int32 UInputCounter::GetTextAmount()
{
	if (nullptr == InputText) { return 0; }

	return FCString::Atoi(*InputText->GetText().ToString());
}
