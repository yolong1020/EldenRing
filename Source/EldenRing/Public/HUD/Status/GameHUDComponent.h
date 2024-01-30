// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Items/ItemTypes.h"
#include "HUD/Status/QuickSlotTypes.h"
#include "GameHUDComponent.generated.h"

class UGameHUD;

UCLASS()
class ELDENRING_API UGameHUDComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	explicit UGameHUDComponent();
	void InitHUD(class UAttributeComponent* const Attribute);
	void SetQuickSlotItem(const EQuickSlotType& SlotType, const TObjectPtr<class UItemObject> ItemObject = nullptr, const EPotionSizeType& PostionSize = EPotionSizeType::EPST_Empty);
	void UpdateStatusBar();

private:
	UPROPERTY()
	TObjectPtr<class UPlayerHUD> m_widget_hud;
};
