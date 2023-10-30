// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimeLineComponent.h"
#include "../Characters/CharacterTypes.h"
#include "Attribute_Delegates.h"
#include "AttributeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASIC_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UAttributeComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ReceiveDamage(float damage);
	void RecoveryHealth(float amount);
	void ConsumeStamina(float amount);

	void RegenerateStat(const EOverlayStatType& stat_type);

	void BindDelegate(FOnRefreshStatBar& function);

	FORCEINLINE float GetHealthPercent()	{ return m_health_current / m_health_max; }
	FORCEINLINE float GetManaPercent()		{ return m_mana_current / m_mana_max; }
	FORCEINLINE float GetStaminaPercent()	{ return m_stamina_current / m_stamina_max; }

	FORCEINLINE float GetHealthCurrent()	{ return m_health_current; }
	FORCEINLINE float GetManaCurrent()		{ return m_mana_current; }
	FORCEINLINE float GetStaminaCurrent()	{ return m_stamina_current; }

	FORCEINLINE float GetHealthMax()		{ return m_health_max; }
	FORCEINLINE float GetManaMax()			{ return m_mana_max; }
	FORCEINLINE float GetStaminaMax()		{ return m_stamina_max; }

	bool IsAlive();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnUpdateRegenerateStat_Health(float curve_value);
	UFUNCTION()
	void OnUpdateRegenerateStat_Mana(float curve_value);
	UFUNCTION()
	void OnUpdateRegenerateStat_Stamina(float curve_value);

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_health_current;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_health_max;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_mana_current;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_mana_max;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_stamina_current;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float m_stamina_max;

	FOnRefreshStatBar	m_delegate_regenerate;
	TArray<FTimeline>	m_timeline_regenerate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveFloat*	m_curve_regenerate;

};
