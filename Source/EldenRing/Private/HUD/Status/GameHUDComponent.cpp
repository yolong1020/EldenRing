// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Status/GameHUDComponent.h"
#include "HUD/Status/PlayerHUD.h"
#include "Items/ItemObject.h"
#include "Macro/DebugMacros.h"

UGameHUDComponent::UGameHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGameHUDComponent::InitHUD(UAttributeComponent* const Attribute)
{
	UClass* widget_class = LoadClass<UPlayerHUD>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/HUD/Status/WBP_PlayerHUD.WBP_PlayerHUD_C'"));
	CHECK_INVALID_PTR(widget_class)

	m_widget_hud = CreateWidget<UPlayerHUD>(GetWorld(), widget_class);
	CHECK_INVALID_PTR(m_widget_hud)

	m_widget_hud->AddToViewport();
	m_widget_hud->InitialStatusBar(Attribute);
}

void UGameHUDComponent::SetQuickSlotItem(const EQuickSlotType& SlotType, const TObjectPtr<UItemObject> ItemObject, const EPotionSizeType& PostionSize)
{
	CHECK_INVALID_PTR(m_widget_hud)
	m_widget_hud->SetQuickSlotItem(SlotType, ItemObject, PostionSize);
}

void UGameHUDComponent::UpdateStatusBar()
{
	CHECK_INVALID_PTR(m_widget_hud)
	m_widget_hud->UpdateStatusBar();
}
