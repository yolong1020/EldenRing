// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Trade/TradeGrid.h"
#include "UI/Trade/TradeInteractionGrid.h"
#include "UI/Inventory/InventorySlotGrid.h"
#include "UI/Inventory/InventoryItem.h"
#include "UI/Structure/InventoryLine.h"
#include "UI/Structure/InventoryTile.h"
#include "System/TradeManager.h"
#include "System/StorageManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/TextBlock.h"
#include "Macro/DebugMacros.h"

void UTradeGrid::InitGridWidget(const bool IsPlayer)
{
	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)
	UStorageManager* storage_mgr = instance->GetSubsystem<UStorageManager>();
	CHECK_INVALID_PTR(storage_mgr)
	UTradeManager* trade_mgr = instance->GetSubsystem<UTradeManager>();
	CHECK_INVALID_PTR(trade_mgr)

	UWidget* widget_slot_grid = GetWidgetFromName(FName("WBP_Trade_SlotGrid"));
	m_widget_slot_grid = Cast<UInventorySlotGrid>(widget_slot_grid);
	CHECK_INVALID_PTR(m_widget_slot_grid)
	m_widget_slot_grid->InitGridSlotWidget();

	UWidget* widget_trade_grid = GetWidgetFromName(FName("WBP_Trade_InteractionGrid"));
	m_widget_Interaction_grid = Cast<UTradeInteractionGrid>(widget_trade_grid);
	CHECK_INVALID_PTR(m_widget_Interaction_grid)
	m_widget_Interaction_grid->InitInteractionGridWidget(IsPlayer, m_margin);
	
	FHUDGoldUpdate callback;
	callback.BindUFunction(this, FName("RefreshGoldAmount"));
	trade_mgr->InitUpdateGoldCallback(IsPlayer, callback);

	if (IsPlayer) { RefreshOwnerName(TEXT("소지품")); }
}

void UTradeGrid::RefreshGoldAmount(const int32& gold_amount)
{
	CHECK_INVALID_PTR(GoldAmount)
	FString string = UKismetTextLibrary::AsCurrency_Integer(gold_amount, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 3).ToString();
	string.RemoveAt(0);

	GoldAmount->SetText(FText::FromString(string));
}

void UTradeGrid::RefreshOwnerName(const FString& Name)
{
	CHECK_INVALID_PTR(OwnerName)
	OwnerName->SetText(FText::FromString(Name));
}

