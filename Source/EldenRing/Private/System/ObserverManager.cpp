// Fill out your copyright notice in the Description page of Project Settings.


#include "System/ObserverManager.h"
#include "System/Event/EventDelegates.h"
#include "Characters/GameCharacter.h"
#include "Characters/NPC/Vender/Vender.h"

void UObserverManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_event.SetNum((int32)EObserverEventType::EOET_Max);
}

void UObserverManager::TriggerEvent(const EObserverEventType& EventType)
{
	if (!m_event.IsValidIndex((int32)EventType)) return;

	if (m_event[(int32)EventType].IsBound())
	{
		m_event[(int32)EventType].Broadcast();
	}
}

void UObserverManager::TriggerDeathEvent(AGameCharacter* const Target)
{
	if (!m_death_event.Contains(Target)) return;

	m_death_event.Find(Target)->Broadcast(Target);
	m_death_event.Remove(Target);
}
