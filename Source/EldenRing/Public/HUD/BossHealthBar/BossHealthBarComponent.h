// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimeLineComponent.h"
#include "BossHealthBarComponent.generated.h"

class UAttributeComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ELDENRING_API UBossHealthBarComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	explicit UBossHealthBarComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override final;
	void InitStatusBar(const FText& BossName, UAttributeComponent* const Attribute);

	void ShowBossHealthBar();
	void HideBossHealthBar();

protected:
	virtual void BeginPlay() override final;

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
	TObjectPtr<class UAttributeComponent> m_attribute;

	UPROPERTY()
	TObjectPtr<class UBossHealthBar> m_health_bar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> m_curve_consume;

	FTimeline		m_tl_consume;
	FTimerHandle	m_delay;
	bool			m_is_delay;
};
