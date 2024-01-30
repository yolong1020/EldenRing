// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Characters/Component/Attribute_Delegates.h"
#include "Components/TimeLineComponent.h"
#include "HealthBarComponent.generated.h"

class UAttributeComponent;

UCLASS()
class ELDENRING_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void InitStatusBar(UAttributeComponent* const Attribute);
	void SetHealthPercent(float percent);

private:
	UFUNCTION()
	void UpdateHealth(const float& Value);

	UFUNCTION()
	void UpdateRecoverHealth(const float& Value);

	UFUNCTION()
	void UpdateConsumedHealth();

	UFUNCTION()
	void StartHealthTL();

private:
	UPROPERTY()
	class UHealthBar* m_widget_healthbar;

	UPROPERTY()
	UAttributeComponent* m_attribute;

#pragma region Timeline
	FTimeline m_tl_consume_health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* m_curve_consume;
#pragma endregion

	bool is_delay_health = false;
};
