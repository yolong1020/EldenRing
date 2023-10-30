// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "../DebugMacros.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	m_timeline_regenerate.SetNum((int32)EOverlayStatType::EOST_End);

	//	Timeline
	FOnTimelineFloat timeline_health_callback;
	FOnTimelineFloat timeline_mana_callback;
	FOnTimelineFloat timeline_stamina_callback;

	timeline_health_callback.BindUFunction(this, FName("OnUpdateRegenerateStat_Health"));
	timeline_mana_callback.BindUFunction(this, FName("OnUpdateRegenerateStat_Mana"));
	timeline_stamina_callback.BindUFunction(this, FName("OnUpdateRegenerateStat_Stamina"));

	m_timeline_regenerate[(int32)EOverlayStatType::EOST_Health].AddInterpFloat(m_curve_regenerate, timeline_health_callback);
	m_timeline_regenerate[(int32)EOverlayStatType::EOST_Mana].AddInterpFloat(m_curve_regenerate, timeline_mana_callback);
	m_timeline_regenerate[(int32)EOverlayStatType::EOST_Stamina].AddInterpFloat(m_curve_regenerate, timeline_stamina_callback);
}

void UAttributeComponent::OnUpdateRegenerateStat_Health(float curve_value)
{
	m_health_current = FMath::Clamp(m_health_current + 0.1f, 0, m_health_max);
	m_delegate_regenerate.Execute(EOverlayStatType::EOST_Health, GetHealthPercent(), true);

	int32 index = (int32)EOverlayStatType::EOST_Health;
	if (m_health_current == m_health_max)
	{
		m_timeline_regenerate[index].Stop();
	}
}

void UAttributeComponent::OnUpdateRegenerateStat_Mana(float curve_value)
{
}

void UAttributeComponent::OnUpdateRegenerateStat_Stamina(float curve_value)
{
	m_stamina_current = FMath::Clamp(m_stamina_current + 0.1f, 0, m_stamina_max);
	m_delegate_regenerate.Execute(EOverlayStatType::EOST_Stamina, GetStaminaPercent(), true);

	int32 index = (int32)EOverlayStatType::EOST_Stamina;
	if (m_stamina_current == m_stamina_max)
	{
		m_timeline_regenerate[index].Stop();
	}
}

void UAttributeComponent::ReceiveDamage(float damage)
{
	if (0 >= damage) { return; }

	m_timeline_regenerate[(int32)EOverlayStatType::EOST_Health].Stop();
	m_health_current = FMath::Clamp(m_health_current - damage, 0, m_health_max);
}

void UAttributeComponent::RecoveryHealth(float amount)
{
	if (0 >= amount) { return; }

	m_health_current = FMath::Clamp(m_health_current + amount, 0, m_health_max);
}

void UAttributeComponent::ConsumeStamina(float amount)
{
	if (0 >= amount) { return; }

	m_timeline_regenerate[(int32)EOverlayStatType::EOST_Stamina].Stop();
	m_stamina_current = FMath::Clamp(m_stamina_current - amount, 0, m_stamina_max);
}

bool UAttributeComponent::IsAlive()
{
	return m_health_current > 0.f;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FTimeline timeline : m_timeline_regenerate)
	{
		timeline.TickTimeline(DeltaTime);
	}
}

void UAttributeComponent::RegenerateStat(const EOverlayStatType& stat_type)
{
	switch (stat_type)
	{
	case EOverlayStatType::EOST_Health:
	{
		int32 index = (int32)EOverlayStatType::EOST_Health;

		if (m_timeline_regenerate[index].IsPlaying()) { break; }
		m_timeline_regenerate[index].PlayFromStart();
	}
	break;
	case EOverlayStatType::EOST_Mana:
	{
		int32 index = (int32)EOverlayStatType::EOST_Mana;

		if (m_timeline_regenerate[index].IsPlaying()) { break; }
		m_timeline_regenerate[index].PlayFromStart();
	}
	break;
	case EOverlayStatType::EOST_Stamina:
	{
		int32 index = (int32)EOverlayStatType::EOST_Stamina;

		if (m_timeline_regenerate[index].IsPlaying()) { break; }
		m_timeline_regenerate[index].PlayFromStart();
	}
	break;
	}
}

void UAttributeComponent::BindDelegate(FOnRefreshStatBar& function)
{
	m_delegate_regenerate = function;
}

