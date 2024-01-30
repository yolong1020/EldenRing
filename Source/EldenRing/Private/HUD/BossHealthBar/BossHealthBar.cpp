// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BossHealthBar/BossHealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Macro/DebugMacros.h"

void UBossHealthBar::SetBossName(const FText& InText)
{
	CHECK_INVALID_PTR(BossName)
	BossName->SetText(InText);
}

void UBossHealthBar::SetHealthPercent(const float& percent)
{
	CHECK_INVALID_PTR(HealthBarCurrent)
	HealthBarCurrent->SetPercent(percent);
}

void UBossHealthBar::UpdateHealth(const float& Value)
{
	CHECK_INVALID_PTR(HealthBarCurrent)
	CHECK_INVALID_PTR(HealthCurrent)

	HealthBarCurrent->SetPercent(Value);
	HealthCurrent->SetValue(Value);
}

const bool UBossHealthBar::UpdateConsumedHealth()
{
	double consume = 100 * (double)HealthConsumeBar->GetPercent();
	double current = 100 * (double)HealthBarCurrent->GetPercent();
	float  value   = FMath::Lerp(consume, current, 0.1) * 0.01;

	HealthConsumeBar->SetPercent(value);

	return (int32)consume == (int32)current;
}
