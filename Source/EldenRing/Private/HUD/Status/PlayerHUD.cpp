// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Status/PlayerHUD.h"
#include "Components/Overlay.h"
#include "Components/PanelSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/Image.h"
#include "Characters/Component/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Items/ItemObject.h"
#include "TimerManager.h"
#include "Macro/DebugMacros.h"

void UPlayerHUD::InitialStatusBar(UAttributeComponent* const attribute)
{
	CHECK_INVALID_PTR(attribute)
	m_attribute = attribute;

	CHECK_INVALID_PTR(HealthBar)
	UCanvasPanelSlot* health_slot = Cast<UCanvasPanelSlot>(HealthBar->Slot);
	health_slot->SetSize(FVector2d(m_attribute->GetHealthMax(), health_slot->GetSize().Y));

	CHECK_INVALID_PTR(ManaBar)
	UCanvasPanelSlot* mana_slot = Cast<UCanvasPanelSlot>(ManaBar->Slot);
	mana_slot->SetSize(FVector2d(m_attribute->GetManaMax(), mana_slot->GetSize().Y));

	CHECK_INVALID_PTR(StaminaBar)
	UCanvasPanelSlot* stamina_slot = Cast<UCanvasPanelSlot>(StaminaBar->Slot);
	stamina_slot->SetSize(FVector2d(m_attribute->GetStaminaMax(), stamina_slot->GetSize().Y));

	//	Timeline
	CHECK_INVALID_PTR(m_curve_consume)
	for (int32 i = 0; i < (int32)EOverlayStatType::EOST_End; ++i)
	{
		FOnTimelineFloat tl_callback;
		FOnUpdateStat consume_callback;
		FOnUpdateRecoverStat recovery_callback;

		FLatentActionInfo delay_callback;
		delay_callback.Linkage			= i;
		delay_callback.CallbackTarget	= this;

		EOverlayStatType type = (EOverlayStatType)i;
		switch (type)
		{
			case EOverlayStatType::EOST_Health: 
				tl_callback.BindUFunction(this, FName("UpdateConsumedHealth"));
				consume_callback.BindUFunction(this, FName("UpdateHealth"));
				recovery_callback.BindUFunction(this, FName("UpdateRecoverHealth"));
				break;
			case EOverlayStatType::EOST_Mana: 
				break;
			case EOverlayStatType::EOST_Stamina: 
				tl_callback.BindUFunction(this, FName("UpdateConsumedStamina"));
				consume_callback.BindUFunction(this, FName("UpdateStamina"));
				recovery_callback.BindUFunction(this, FName("UpdateRecoverStamina"));
				break;
		}

		m_attribute->InitUpdateCallback(type, consume_callback, recovery_callback);
		m_tl_consume[i].AddInterpFloat(m_curve_consume, tl_callback);
	}
}

void UPlayerHUD::UpdateStatusBar()
{
	UCanvasPanelSlot* health_slot = Cast<UCanvasPanelSlot>(HealthBar->Slot);
	health_slot->SetSize(FVector2d(m_attribute->GetHealthMax(), health_slot->GetSize().Y));

	float health_percent = m_attribute->GetHealthPercent();
	HealthConsumeBar->SetPercent(health_percent);
	HealthBarCurrent->SetPercent(health_percent);
	HealthCurrent->SetValue(health_percent);

	UCanvasPanelSlot* mana_slot = Cast<UCanvasPanelSlot>(ManaBar->Slot);
	mana_slot->SetSize(FVector2d(m_attribute->GetManaMax(), mana_slot->GetSize().Y));

	float mana_percent = m_attribute->GetManaPercent();
	ManaConsumeBar->SetPercent(mana_percent);
	ManaBarCurrent->SetPercent(mana_percent);
	ManaCurrent->SetValue(mana_percent);

	UCanvasPanelSlot* stamina_slot = Cast<UCanvasPanelSlot>(StaminaBar->Slot);
	stamina_slot->SetSize(FVector2d(m_attribute->GetStaminaMax(), stamina_slot->GetSize().Y));

	float stamina_percent = m_attribute->GetStaminaPercent();
	StaminaConsumeBar->SetPercent(stamina_percent);
	StaminaBarCurrent->SetPercent(stamina_percent);
	StaminaCurrent->SetValue(stamina_percent);

	m_attribute->RecoverStamina();
}

void UPlayerHUD::SetQuickSlotItem(const EQuickSlotType& Type, const TObjectPtr<UItemObject> ItemObject, const EPotionSizeType& PotionSize)
{
	UImage* image_component = nullptr;

	switch (Type)
	{
		case EQuickSlotType::EQST_Top:
		case EQuickSlotType::EQST_Bottom:
		{
			image_component = (Type == EQuickSlotType::EQST_Top) ? QuickSlot_Top_Item : QuickSlot_Bottom_Item;

			if (m_potion_matarials.IsValidIndex((int32)PotionSize) == false || m_potion_matarials[(int32)PotionSize] == nullptr) { return; }

			image_component->SetVisibility(ESlateVisibility::Visible);
			image_component->SetBrushFromMaterial(m_potion_matarials[(int32)PotionSize]);
			image_component->SetDesiredSizeOverride(FVector2D(1, 1));
		} break;
		case EQuickSlotType::EQST_Left:
		case EQuickSlotType::EQST_Right:
		{
			image_component = (Type == EQuickSlotType::EQST_Left) ? QuickSlot_Left_Item : QuickSlot_Right_Item;

			if (nullptr == ItemObject) { image_component->SetVisibility(ESlateVisibility::Hidden); return; }

			image_component->SetVisibility(ESlateVisibility::Visible);
			image_component->SetBrushFromMaterial(ItemObject->GetIconImage(false).Get());
			image_component->SetDesiredSizeOverride(FVector2D(1, 1));
		} break;
	}
}

void UPlayerHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_is_delay.SetNum((int32)EOverlayStatType::EOST_End);
	m_tl_consume.SetNum((int32)EOverlayStatType::EOST_End);
	m_timer.SetNum((int32)EOverlayStatType::EOST_End);

	CHECK_INVALID_PTR(QuickSlot_Top_Item)
	CHECK_INVALID_PTR(QuickSlot_Bottom_Item)
	CHECK_INVALID_PTR(QuickSlot_Left_Item)
	CHECK_INVALID_PTR(QuickSlot_Right_Item)

	QuickSlot_Top_Item->SetVisibility(ESlateVisibility::Hidden);
	QuickSlot_Bottom_Item->SetVisibility(ESlateVisibility::Hidden);
	QuickSlot_Left_Item->SetVisibility(ESlateVisibility::Hidden);
	QuickSlot_Right_Item->SetVisibility(ESlateVisibility::Hidden);

	m_potion_matarials.Reserve(StaticEnum<EPotionSizeType>()->GetMaxEnumValue());
	m_potion_matarials.Add(LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/Items/Items/Potion/MI_Potion_Empty.MI_Potion_Empty")));
	m_potion_matarials.Add(LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/Items/Items/Potion/MI_Potion_Less.MI_Potion_Less")));
	m_potion_matarials.Add(LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/Items/Items/Potion/MI_Potion_Half.MI_Potion_Half")));
	m_potion_matarials.Add(LoadObject<UMaterialInterface>(nullptr, *FString("/Game/EldenRing/Materials/Items/Items/Potion/MI_Potion_Full.MI_Potion_Full")));
}

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (int32 i = 0; i < (int32)EOverlayStatType::EOST_End; ++i)
	{
		m_tl_consume[i].TickTimeline(InDeltaTime);
	}
}

void UPlayerHUD::StartHealthTL()
{
	m_tl_consume[(int32)EOverlayStatType::EOST_Health].PlayFromStart();
}

void UPlayerHUD::StartStaminaTL()
{
	m_tl_consume[(int32)EOverlayStatType::EOST_Stamina].PlayFromStart();
}

void UPlayerHUD::UpdateHealth(const float& Value)
{
	float percent = Value / m_attribute->GetHealthMax();
	HealthBarCurrent->SetPercent(percent);
	HealthCurrent->SetValue(percent);

	UpdateStat(EOverlayStatType::EOST_Health);
}

void UPlayerHUD::UpdateMana(const float& Value)
{
}

void UPlayerHUD::UpdateStamina(const float& Value)
{
	float percent = Value / m_attribute->GetStaminaMax();
	StaminaBarCurrent->SetPercent(percent);
	StaminaCurrent->SetValue(percent);

	UpdateStat(EOverlayStatType::EOST_Stamina);
}

void UPlayerHUD::UpdateStat(const EOverlayStatType& Type)
{
	if (m_is_delay[(int32)Type]) return;
		
	FTimerManager& timer = GetWorld()->GetTimerManager();
	timer.ClearTimer(m_timer[(int32)Type]);

	switch (Type)
	{
		case EOverlayStatType::EOST_Health: timer.SetTimer(m_timer[(int32)Type], this, &UPlayerHUD::StartHealthTL, 1.4f); break;
		case EOverlayStatType::EOST_Mana: break;
		case EOverlayStatType::EOST_Stamina: timer.SetTimer(m_timer[(int32)Type], this, &UPlayerHUD::StartStaminaTL, 1.4f); break;
	}

	m_is_delay[(int32)Type] = true;
}

void UPlayerHUD::UpdateRecoverHealth(const float& Value)
{
	float percent = (Value / m_attribute->GetHealthMax());
	HealthBarCurrent->SetPercent(percent);
	HealthConsumeBar->SetPercent(percent);
	HealthCurrent->SetValue(percent);
}

void UPlayerHUD::UpdateRecoverStamina(const float& Value)
{
	float percent = Value / m_attribute->GetStaminaMax();
	StaminaBarCurrent->SetPercent(percent);
	StaminaConsumeBar->SetPercent(percent);
	StaminaCurrent->SetValue(percent);
}

void UPlayerHUD::UpdateConsumedHealth()
{
	double consume = 100 * (double)HealthConsumeBar->GetPercent();
	double current = 100 * (double)HealthBarCurrent->GetPercent();

	UpdateConsumedStat(EOverlayStatType::EOST_Health, consume, current);
}

void UPlayerHUD::UpdateConsumedStamina(float Value)
{
	int32 consume	= 100 * (double)StaminaConsumeBar->GetPercent();
	int32 current	= 100 * (double)StaminaBarCurrent->GetPercent();

	UpdateConsumedStat(EOverlayStatType::EOST_Stamina, consume, current);
}

void UPlayerHUD::UpdateConsumedStat(const EOverlayStatType& Type, const double& Consume, const double& Current)
{
	if ((int32)Consume == (int32)Current)
	{
		m_tl_consume[(int32)Type].Stop();
		m_is_delay[(int32)Type] = false;
	}
	else
	{
		float value = FMath::Lerp(Consume, Current, 0.1) * 0.01;

		switch (Type)
		{
			case EOverlayStatType::EOST_Health:		HealthConsumeBar->SetPercent(value);	break;
			case EOverlayStatType::EOST_Mana:		ManaConsumeBar->SetPercent(value);		break;
			case EOverlayStatType::EOST_Stamina:	StaminaConsumeBar->SetPercent(value);	break;
		}
	}
}
