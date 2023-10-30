// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Status/GameHUD.h"
#include "Components/Overlay.h"
#include "Components/PanelSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/AttributeComponent.h"
#include "Components/Image.h"
#include "Items/ItemObject.h"
#include "TimerManager.h"
#include "../DebugMacros.h"

void UGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	m_tl_consume_health.TickTimeline(InDeltaTime);
	m_tl_consume_mana.TickTimeline(InDeltaTime);
	m_tl_consume_stamina.TickTimeline(InDeltaTime);
}

void UGameHUD::OnUpdateHealthStatusBar(float value)
{
	if (0 == value) { return; }
	UpdateStatusTimeline(EOverlayStatType::EOST_Health);
}

void UGameHUD::OnUpdateManaStatusBar(float value)
{
	if (0 == value) { return; }
	UpdateStatusTimeline(EOverlayStatType::EOST_Mana);
}

void UGameHUD::OnUpdateStaminaStatusBar(float value)
{
	if (0 == value) { return; }
	UpdateStatusTimeline(EOverlayStatType::EOST_Stamina);
}

void UGameHUD::UpdateStatusTimeline(const EOverlayStatType& type)
{
	float		  goal			= 0;
	float		  current		= 0;
	UProgressBar* progress_bar	= nullptr;
	FTimeline	  time_line;

	switch (type)
	{
		case EOverlayStatType::EOST_Health: {
			goal			= HealthBarCurrent->GetPercent();
			HealthConsumeBar->SetPercent(FMath::Lerp(HealthConsumeBar->GetPercent(), goal, 0.05f));

			current			= HealthConsumeBar->GetPercent();
			progress_bar	= HealthConsumeBar;
			time_line		= m_tl_consume_health;
	
		} break;
		case EOverlayStatType::EOST_Mana: {
			goal			= ManaBarCurrent->GetPercent();
			ManaConsumeBar->SetPercent(FMath::Lerp(ManaConsumeBar->GetPercent(), goal, 0.05f));

			current			= ManaConsumeBar->GetPercent();
			progress_bar	= ManaConsumeBar;
			time_line		= m_tl_consume_mana;
		} break;
		case EOverlayStatType::EOST_Stamina: {
			goal			= StaminaBarCurrent->GetPercent();
			StaminaConsumeBar->SetPercent(FMath::Lerp(StaminaConsumeBar->GetPercent(), goal, 10.f));

			current			= StaminaConsumeBar->GetPercent();
			progress_bar	= StaminaConsumeBar;
			time_line		= m_tl_consume_stamina;

			if (int(goal * 100.f) >= int(current * 100.f))
			{
				time_line.Stop();
				m_attribute->RegenerateStat(type);
			}
		} break;
		default: return;
	}

}

void UGameHUD::InitialStatusBar(UAttributeComponent* const attribute)
{
	CHECK_INVALID(attribute)
	m_attribute = attribute;

	CHECK_INVALID(HealthBar)
	UCanvasPanelSlot*	health_slot		= Cast<UCanvasPanelSlot>(HealthBar->Slot);
	FVector2d			health_bar_size	= health_slot->GetSize();
	health_slot->SetSize(FVector2d(m_attribute->GetHealthMax(), health_bar_size.Y));

	CHECK_INVALID(ManaBar)
	UCanvasPanelSlot* mana_slot		= Cast<UCanvasPanelSlot>(ManaBar->Slot);
	FVector2d		  mana_bar_size = mana_slot->GetSize();
	mana_slot->SetSize(FVector2d(m_attribute->GetManaMax(), mana_bar_size.Y));

	CHECK_INVALID(StaminaBar)
	UCanvasPanelSlot* stamina_slot = Cast<UCanvasPanelSlot>(StaminaBar->Slot);
	FVector2d		  stamina_bar_size = stamina_slot->GetSize();
	stamina_slot->SetSize(FVector2d(m_attribute->GetStaminaMax(), stamina_bar_size.Y));

	//	Timeline
	CHECK_INVALID(m_curve_consume)
	FOnTimelineFloat tl_health_callback;
	FOnTimelineFloat tl_mana_callback;
	FOnTimelineFloat tl_stamina_callback;

	tl_health_callback.BindUFunction(this, FName("OnUpdateHealthStatusBar"));
	m_tl_consume_health.AddInterpFloat(m_curve_consume, tl_health_callback);

	tl_mana_callback.BindUFunction(this, FName("OnUpdateManaStatusBar"));
	m_tl_consume_mana.AddInterpFloat(m_curve_consume, tl_mana_callback);

	tl_stamina_callback.BindUFunction(this, FName("OnUpdateStaminaStatusBar"));
	m_tl_consume_stamina.AddInterpFloat(m_curve_consume, tl_stamina_callback);

	m_regenerate_delegate = m_regenerate_delegate.CreateUFunction(this, FName("RefreshStatBar"));
	m_attribute->BindDelegate(m_regenerate_delegate);

	CHECK_INVALID(QuickSlot_Top_Item)
	CHECK_INVALID(QuickSlot_Bottom_Item)
	CHECK_INVALID(QuickSlot_Left_Item)
	CHECK_INVALID(QuickSlot_Right_Item)

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

void UGameHUD::RefreshStatBar(const EOverlayStatType& type, const float& percent, const bool& is_regenereate)
{
	UProgressBar*	stat_bar			= nullptr;
	UProgressBar*	consume_bar			= nullptr;
	USlider*		indicator_slider	= nullptr;

	switch (type)
	{
		case EOverlayStatType::EOST_Health: {
			stat_bar		 = HealthBarCurrent;
			consume_bar		 = HealthConsumeBar;
			indicator_slider = HealthCurrent;

			if (!is_regenereate && false == m_tl_consume_health.IsPlaying()) { 
				m_tl_consume_health.PlayFromStart(); 
			}
		} break;
		case EOverlayStatType::EOST_Mana: {
			stat_bar		 = ManaBarCurrent;
			consume_bar		 = ManaConsumeBar;
			indicator_slider = ManaCurrent;

			if (!is_regenereate && false == m_tl_consume_mana.IsPlaying()) { 
				m_tl_consume_mana.PlayFromStart(); 
			}
		} break;
		case EOverlayStatType::EOST_Stamina: {
			stat_bar		 = StaminaBarCurrent;
			consume_bar		 = StaminaConsumeBar;
			indicator_slider = StaminaCurrent;

			if (!is_regenereate && false == m_tl_consume_stamina.IsPlaying()) { 
				m_tl_consume_stamina.PlayFromStart();
			}
		} break;
		default: return;
	}

	stat_bar->SetPercent(percent);
	indicator_slider->SetValue(percent);
	if (is_regenereate) {
		consume_bar->SetPercent(percent);
	}
}

void UGameHUD::SetQuickSlotItem(const EQuickSlotType& slot_type, UItemObject* const item_object, const EPotionSizeType& potion_size)
{
	UImage* image_component = nullptr;
	switch (slot_type)
	{
	case EQuickSlotType::EQST_Top:
	case EQuickSlotType::EQST_Bottom:
	{
		image_component = (slot_type == EQuickSlotType::EQST_Top) ? QuickSlot_Top_Item : QuickSlot_Bottom_Item;

		if (m_potion_matarials.IsValidIndex((int32)potion_size) == false || m_potion_matarials[(int32)potion_size] == nullptr) { return; }

		image_component->SetVisibility(ESlateVisibility::Visible);
		image_component->SetBrushFromMaterial(m_potion_matarials[(int32)potion_size]);
		image_component->SetDesiredSizeOverride(FVector2D(1, 1));
		break;
	}
	case EQuickSlotType::EQST_Left:
	case EQuickSlotType::EQST_Right:
	{
		image_component = (slot_type == EQuickSlotType::EQST_Left) ? QuickSlot_Left_Item : QuickSlot_Right_Item;

		if (nullptr == item_object) { image_component->SetVisibility(ESlateVisibility::Hidden); return; }

		image_component->SetVisibility(ESlateVisibility::Visible);
		image_component->SetBrushFromMaterial(item_object->GetIconImage(false));
		image_component->SetDesiredSizeOverride(FVector2D(1, 1));
		break;
	}
	}
}
