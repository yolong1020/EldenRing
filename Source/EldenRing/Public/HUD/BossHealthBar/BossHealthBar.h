// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHealthBar.generated.h"

UCLASS()
class ELDENRING_API UBossHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetBossName(const FText& InText);
	void SetHealthPercent(const float& percent);
	void UpdateHealth(const float& Value);
	const bool UpdateConsumedHealth();
	
private:
	UPROPERTY()
	class UAttributeComponent* m_attribute;

#pragma region Health
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* BossName;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UOverlay* HealthBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UProgressBar* HealthConsumeBar;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UProgressBar* HealthBarCurrent;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class USlider* HealthCurrent;
#pragma endregion
};
