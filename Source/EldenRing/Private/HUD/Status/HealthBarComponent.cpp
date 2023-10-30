// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Status/HealthBarComponent.h"
#include "HUD/Status/HealthBar.h"
#include "Components/ProgressBar.h"
#include "../DebugMacros.h"

void UHealthBarComponent::SetHealthPercent(float percent)
{
	if (nullptr == m_widget_healthbar)
	{
		m_widget_healthbar = Cast<UHealthBar>(GetUserWidgetObject());
		CHECK_INVALID(m_widget_healthbar)
	}
	
	CHECK_INVALID(m_widget_healthbar->HealthBar)
	
	m_widget_healthbar->HealthBar->SetPercent(percent);
}
