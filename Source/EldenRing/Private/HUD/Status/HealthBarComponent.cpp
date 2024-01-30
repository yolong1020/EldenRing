// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Status/HealthBarComponent.h"
#include "HUD/Status/HealthBar.h"
#include "Components/ProgressBar.h"
#include "Characters/Component/AttributeComponent.h"
#include "Macro/DebugMacros.h"

void UHealthBarComponent::InitStatusBar(UAttributeComponent* const Attribute)
{
	m_attribute = Attribute;

	FOnUpdateStat health_consume;
	health_consume.BindUFunction(this, FName("UpdateHealth"));
	FOnUpdateRecoverStat health_recovery;
	health_recovery.BindUFunction(this, FName("UpdateRecoverHealth"));
	m_attribute->InitUpdateCallback(EOverlayStatType::EOST_Health, health_consume, health_recovery);
}

void UHealthBarComponent::SetHealthPercent(float percent)
{
	if (nullptr == m_widget_healthbar)
	{
		m_widget_healthbar = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	CHECK_INVALID_PTR(m_widget_healthbar)
	m_widget_healthbar->SetPercent(percent);
}

void UHealthBarComponent::UpdateHealth(const float& Value)
{

}

void UHealthBarComponent::UpdateRecoverHealth(const float& Value)
{
}

void UHealthBarComponent::UpdateConsumedHealth()
{
}

void UHealthBarComponent::StartHealthTL()
{
}
