// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Status/HealthBar.h"
#include "Macro/DebugMacros.h"
#include "Components/ProgressBar.h"

void UHealthBar::SetPercent(const float& Percent)
{
	CHECK_INVALID_PTR(HealthBar)
	HealthBar->SetPercent(Percent);
}
