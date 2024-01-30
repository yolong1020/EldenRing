// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PopUp/PopUp.h"
#include "UI/Button/CommonButton.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "System/TradeManager.h"
#include "Items/ItemObject.h"
#include "Macro/DebugMacros.h"

void UPopUp::UpdataTwoButtonForTrade(const EPopupType& PopupType, const TObjectPtr<UItemObject> ItemObject, const FPopupActiveBtn& OnActive, const FPopupCancelBtn& OnCancel)
{
	if ("None" == Active->GetName() || "None" == Cancel->GetName() || "None" == Confirm->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Popup Data"))
	}

	Active->SetVisibility(ESlateVisibility::Visible);
	Cancel->SetVisibility(ESlateVisibility::Visible);
	Confirm->SetVisibility(ESlateVisibility::Collapsed);

	m_on_active = OnActive;
	m_on_cancel = OnCancel;
	const FItemData* item_data = ItemObject->GetItemData();
	CHECK_INVALID_PTR(item_data)
	FString cost = UKismetTextLibrary::AsCurrency_Integer((PopupType == EPopupType::EPT_Sell) ? item_data->Cost * 0.5 : item_data->Cost, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 3).ToString();
	cost.RemoveAt(0);

	UpdateTwoBtnTextForTrade(item_data->ItemName, cost, PopupType);
	AddToViewport();
}

void UPopUp::UpdateTwoButtonForExitGame(const EPopupType& PopupType, const FPopupActiveBtn& OnActive, const FPopupCancelBtn& OnCancel)
{
	if ("None" == Active->GetName() || "None" == Cancel->GetName() || "None" == Confirm->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Popup Data"))
	}

	Active->SetVisibility(ESlateVisibility::Visible);
	Cancel->SetVisibility(ESlateVisibility::Visible);
	Confirm->SetVisibility(ESlateVisibility::Collapsed);

	m_on_active = OnActive;
	m_on_cancel = OnCancel;

	UpdateTwoBtnText(PopupType);
}

void UPopUp::UpdateOneButtonLayout(const EPopupType& PopupType, const FPopupConfirmlBtn& OnConfirm)
{
	if ("None" == Active->GetName() || "None" == Cancel->GetName() || "None" == Confirm->GetName())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Popup Data"))
	}

	Active->SetVisibility(ESlateVisibility::Collapsed);
	Cancel->SetVisibility(ESlateVisibility::Collapsed);
	Confirm->SetVisibility(ESlateVisibility::Visible);

	m_on_confirm = OnConfirm;

	UpdateOneBtnText(PopupType);
	AddToViewport();
}

void UPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CHECK_INVALID_PTR(Active)
	CHECK_INVALID_PTR(Cancel)
	CHECK_INVALID_PTR(Confirm)

	FScriptDelegate func_delegate;
	func_delegate.BindUFunction(this, "OnActive");
	Active->BindButtonFunc(func_delegate);

	func_delegate.Unbind();
	func_delegate.BindUFunction(this, "OnCancel");
	Cancel->BindButtonFunc(func_delegate);

	func_delegate.Unbind();
	func_delegate.BindUFunction(this, "OnConfirm");
	Confirm->BindButtonFunc(func_delegate);
}

void UPopUp::OnActive()
{
	if (m_on_active.IsBound()) { m_on_active.Execute(); }
	RemoveFromParent();
}

void UPopUp::OnCancel()
{
	if (m_on_cancel.IsBound()) { m_on_cancel.Execute(); }
	RemoveFromParent();
}

void UPopUp::OnConfirm()
{
	if (m_on_confirm.IsBound()) { m_on_confirm.Execute(); }
	RemoveFromParent();
}
